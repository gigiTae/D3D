#include "RendererPCH.h"
#include "D3DRenderer.h"
#include "SimpleMesh.h"
#include "CameraObject.h"

D3DRenderer::D3DRenderer()
	:m_d3dDevice(nullptr)
	, m_hWnd(nullptr)
	, m_d3dDeviceContext(nullptr)
	, m_featureLevel()
	, m_swapChain()
	, m_enable4xMass(false)
	, m_d3dRenderTargetView()
	, m_depthStencilView()
	, m_depthStencilBuffer()
	, m_worldViewProjMatrix()
	, m_worldMatrix()
	, m_constantBuffer(nullptr)
	, m_screenWidth(0)
	, m_screenHeight(0)
	,m_bufferContainer()
{
	m_mesh = new SimpleMesh();
}

D3DRenderer::~D3DRenderer()
{
	delete m_mesh;


}



bool D3DRenderer::Initialize(HWND hWnd, int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_hWnd = hWnd;

	// 카메라 생성
	m_mainCamera = std::make_unique<CameraObject>();
	XMVECTOR cameraPosition = XMVectorSet(10.f, 20.f, 10.f, 1.f);
	m_mainCamera->Initialize(m_screenWidth, m_screenHeight, cameraPosition);

	InitializeD3D();
	InitializePipeLine();

	CreateBuffer();

	return true;
}

void D3DRenderer::Finalize()
{
	m_mainCamera->Finalize();


	CoUninitialize();
}

void D3DRenderer::ClearScreen()
{
	float arr[4]{ 0.f,0.f,0.f,1.f };

	m_d3dDeviceContext->ClearRenderTargetView(m_d3dRenderTargetView.Get(),arr);

	// 깊이 버퍼를 1. Of로, 스텐실 버퍼를 0으로 지운다.
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void D3DRenderer::Render()
{
	ClearScreen();

	SetWorldViewProjMatrix();
	// 그림을 그려보자 

	m_d3dDeviceContext->RSSetState(m_rasterizerState[0].Get());
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///// Grid
	// 버텍스 버퍼
	auto vIter = m_bufferContainer.find(L"vGrid");
	assert(vIter != m_bufferContainer.end());
	auto vertex = vIter->second.Get();

	UINT stride = sizeof(DM::Vertex1);
	UINT offset = 0;

	m_d3dDeviceContext->IASetVertexBuffers(
		0, 1, &vertex, &stride, &offset);

	// 인덱스 버퍼
	auto iter = m_bufferContainer.find(L"iGrid");
	auto index = iter->second.Get();
	m_d3dDeviceContext->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

	m_d3dDeviceContext->DrawIndexed(58806, 0, 0);
	
	/// BOX
	{

		m_d3dDeviceContext->RSSetState(m_rasterizerState[1].Get());
		m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		auto vIter = m_bufferContainer.find(L"vBox");
		assert(vIter != m_bufferContainer.end());
		auto vertex = vIter->second.Get();

		UINT stride = sizeof(DM::Vertex1);
		UINT offset = 0;

		m_d3dDeviceContext->IASetVertexBuffers(
			0, 1, &vertex, &stride, &offset);

		// 인덱스 버퍼
		auto iter = m_bufferContainer.find(L"iBox");
		auto index = iter->second.Get();
		m_d3dDeviceContext->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

		m_d3dDeviceContext->DrawIndexed(36, 0, 0);
	}

	HR(m_swapChain->Present(0, 0));
}

void D3DRenderer::CreateBox()
{
	using namespace DirectX;

	auto vertex = DM::Geometry::GetBoxVertices();
	auto index = DM::Geometry::GetBoxIndices();

	/// 상수 버퍼
	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(DM::Vertex1) * vertex.size(); // 생성할 정점 버퍼의 크기
	BF.Usage = D3D11_USAGE_DEFAULT;  // 버퍼가 쓰이는 방식 
	BF.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점 버퍼
	BF.CPUAccessFlags = 0;
	BF.MiscFlags = 0;
	BF.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SD{};
	SD.pSysMem = vertex.data();

	ID3D11Buffer* mVB;

	HR(m_d3dDevice->CreateBuffer(&BF, &SD, &mVB));
	m_bufferContainer.insert(std::make_pair(L"vBox", mVB));

	// 색인과 색인 버퍼 생성	

	// 색인 버퍼를 서술하는 구조체를 채운다.
	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * index.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// 색인 버퍼를 초기화할 자료를 지정한다.
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = index.data();

	// 색인 버퍼를 생성한다.
	ID3D11Buffer* mIB;
	HR(m_d3dDevice->CreateBuffer(&ibd, &initData, &mIB));
	m_bufferContainer.insert(std::make_pair(L"iBox", mIB));

}

void D3DRenderer::SetWorldViewProjMatrix()
{
	// 최종 행렬
	XMMATRIX worldviewProjMatrix = GetWorldViewProjMatrix();

	/// 상수버퍼에서는 전치행렬을 보내주어야한다!!!!!!!!!!!
	worldviewProjMatrix = XMMatrixTranspose(worldviewProjMatrix);

	DirectX::XMStoreFloat4x4(&m_worldViewProjMatrix, worldviewProjMatrix);

	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	assert(m_constantBuffer);

	m_d3dDeviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 포인터를 가져옵니다.
	XMFLOAT4X4* dataPtr = static_cast<XMFLOAT4X4*>(mappedResource.pData);

	// 상수 버퍼에 업데이트할 데이터를 복사합니다.
	*dataPtr = m_worldViewProjMatrix;

	assert(m_constantBuffer);
	// 상수 버퍼를 언맵합니다.
	m_d3dDeviceContext->Unmap(m_constantBuffer, 0);
}

DirectX::XMMATRIX D3DRenderer::GetWorldViewProjMatrix()
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_worldMatrix);
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();
	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();
 
	// 최종 행렬
	return worldMatrix * viewMatrix * projectMatrix;
}

bool D3DRenderer::InitializeD3D()
{
	/// ==================================
	///          Direct3D 초기화 
	/// ==================================

	// Direct3D 초기화 8가지 단계

	// 1. D3D11CreateDevice 함수를 이용해서 장치, 즉 ID3D11 Device 인터페이스와
	// 장치 문맥, 즉 ID3D11DeviceContext 인터 페이스를 생성한다. 

	UINT createDeviceFlags = 0;

#if defined (DEBUG) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // 디버그 용도
#endif

	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&m_d3dDevice,
		&m_featureLevel,
		&m_d3dDeviceContext
	);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (m_featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// 2. ID3D11Device::CheckMultisampleQualityLevels 메서드를 이용해서 
	// 4XMSAA 품질 수준 지원 여부를 점검한다.

	UINT m4xMsaaQuality{};

	HR(m_d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));

	assert(m4xMsaaQuality > 0);

	// 3. 생성할 교환 사슬의 특성을 서술하는 DXGI_SWAP_CHAIN_DESC 구조체를 채운다.

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = m_screenWidth;
	sd.BufferDesc.Height = m_screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4XMASS를 사용하는가?
	if (m4xMsaaQuality)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}


	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// 4. 장치를 생성하는데 사용했던 IDXGIFactory 인터페이스를 질의해서 IDXGISwapChain 인스턴스를 생성한다.

	IDXGIDevice* dxgiDevice = nullptr;
	HR(m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, &m_swapChain));

	// COM 인터페이스 해제
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// 5. 교환 사슬의 후면 버퍼에 대한 렌더 대상 뷰를 생성한다.
	ID3D11Texture2D* backBuffer = nullptr;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	assert(backBuffer);

	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_d3dRenderTargetView));

	backBuffer->Release();

	// 6. 깊이, 스텐실 버퍼와 그에 연결되는 깊이,스텐실 뷰를 생성한다.
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = m_screenWidth;
	depthStencilDesc.Height = m_screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4X MSAA를 사용하는가 ? 반드시 교환 사슬 MSAA 설정과 일치해야함
	if (m4xMsaaQuality)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer));

	assert(m_depthStencilBuffer);
	HR(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView));

	// 7. 랜더 대상 뷰와 깊이,스텐실 뷰를 Direct3D가 사용할 수 있도록 렌더링 파이프라인의 출력 병합기 단계에 묶는다.

	/// ComPtr문제인가 ? OMSetRenderTargets에 함수 호출뒤에 NULL이 들어간다 ???? 참조가 변하나 COMPTR 공부를 더해야할듯
	ID3D11RenderTargetView* renderTargetView = m_d3dRenderTargetView.Get();

	m_d3dDeviceContext->OMSetRenderTargets(1, &renderTargetView, m_depthStencilView.Get());

	// 8. 뷰포트를 설정한다. 뷰포트 2개설정하면 화면분할 가능 

	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width = static_cast<float>(m_screenWidth);
	vp.Height = static_cast<float>(m_screenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_d3dDeviceContext->RSSetViewports(1, &vp);

	// 래스터라이즈 설정

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; // 와이어 프레임 모드
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID; // 삼각형 채우기 모드 
	//rasterizerDesc.CullMode = D3D11_CULL_BACK; // 후면 삼각현 제외
	rasterizerDesc.CullMode = D3D11_CULL_NONE;  // 카메라에 대한 양면 랜더링 활성화
	rasterizerDesc.FrontCounterClockwise = false; // 시계 방향으로 그려지는 삼각형
	rasterizerDesc.DepthClipEnable = true; // 깊이 클리핑 활성화

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState[0]);

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState[1]);

	m_d3dDeviceContext->RSSetState(m_rasterizerState[0].Get());

	return true;
}

bool D3DRenderer::InitializePipeLine()
{
	HRESULT hr = S_OK;	

	ID3D10Blob* vertexShader = nullptr;
	ID3D10Blob* pixelShader = nullptr;

	std::wstring vertexShaderPath = L"C:\\Users\\User\\Desktop\\D3D\\D3D\\Direct3D\\App\\VertexShader.hlsl";

	// 쉐이더 불러오기
	HR(D3DCompileFromFile(L"VertexShader.hlsl", 0, 0
		, "main", "vs_5_0", 0, 0, &vertexShader, 0));

	HR(D3DCompileFromFile(L"PixelShader.hlsl", 0, 0
		, "main", "ps_5_0", 0, 0, &pixelShader, 0));

	// 쉐이더 생성
	HR(m_d3dDevice->CreateVertexShader(
		vertexShader->GetBufferPointer(),
		vertexShader->GetBufferSize(),
		NULL, &m_vertexShader));

	HR(m_d3dDevice->CreatePixelShader(
		pixelShader->GetBufferPointer(),
		pixelShader->GetBufferSize(),
		NULL, &m_pixelShader));

	// 쉐이더 연결
	m_d3dDeviceContext->VSSetShader(m_vertexShader.Get(), 0, 0);
	m_d3dDeviceContext->PSSetShader(m_pixelShader.Get(), 0, 0);

	// 입력 레이아웃 객체 생성
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HR(m_d3dDevice->CreateInputLayout(ied, 2, vertexShader->GetBufferPointer()
		, vertexShader->GetBufferSize(), &m_inputLayout[0]));

	m_d3dDeviceContext->IASetInputLayout(m_inputLayout[0].Get());


	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

void D3DRenderer::CreateBuffer()
{
	/// ====== 버퍼들을 생성한다 =========

	/// 정점 버퍼 생성 
	 //그리드 생성
	CreateGrid(100, 100, 100, 100);
	
	CreateBox();

	// 상수 버퍼 생성
	CreateContantBuffer();
}

void D3DRenderer::CreateGrid(float width, float depth, UINT m, UINT n)
{
	auto& mesh = *m_mesh;

	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	// 정점을 만든다
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	mesh.vertices.resize(vertexCount);

	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - (i * dz);
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + (j * dx);

			mesh.vertices[i * n + j].position = XMFLOAT3(x, 0.0f, z);
			mesh.vertices[i * n + j].color = XMFLOAT4(0.f, 1.f, 0.f, 0.f);
			// 조명에 쓰이는 특성
			 
			// 텍스처 적용에 쓰이는 특성
		}
	}

	// 색인을 만든다
	mesh.indices.resize(faceCount * 3); // 면당 색인 3개

	// 각 낱칸을 훑으면서 색인을 계산하다.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			mesh.indices[k] = i * n + j;
			mesh.indices[k + 1] = i * n + j + 1;
			mesh.indices[k + 2] = (i + 1) * n + j;
			mesh.indices[k + 3] = (i + 1) * n + j;
			mesh.indices[k + 4] = i * n + j + 1;
			mesh.indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // 다음 낱같은으로 
		}
	}

	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(DM::Vertex1) * m_mesh->vertices.size(); // 생성할 정점 버퍼의 크기
	BF.Usage = D3D11_USAGE_DEFAULT;  // 버퍼가 쓰이는 방식 
	BF.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점 버퍼
	BF.CPUAccessFlags = 0;
	BF.MiscFlags = 0;
	BF.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SD{};
	SD.pSysMem = m_mesh->vertices.data();

	ID3D11Buffer* mVB;

	// 생성한 버퍼를 저장한다
	HR(m_d3dDevice->CreateBuffer(&BF, &SD, &mVB));

	m_bufferContainer.insert(std::make_pair(L"vGrid", mVB));

	UINT stride = sizeof(DM::Vertex1);
	UINT offset = 0;

	// 색인과 색인 버퍼 생성

	// 색인 버퍼를 서술하는 구조체를 채운다.
	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_mesh->indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// 색인 버퍼를 초기화할 자료를 지정한다.
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = m_mesh->indices.data();

	// 색인 버퍼를 생성한다.
	ID3D11Buffer* mIB;
	HR(m_d3dDevice->CreateBuffer(&ibd, &initData, &mIB));

	m_bufferContainer.insert(std::make_pair(L"iGrid", mIB));

}

void D3DRenderer::CreateContantBuffer()
{
	// 월드 
	XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	// 카메라 
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();

	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();

	// 최종 행렬
	XMMATRIX finalMatrix = worldMatrix * viewMatrix * projectMatrix;

	DirectX::XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
	DirectX::XMStoreFloat4x4(&m_worldViewProjMatrix, finalMatrix);

	D3D11_BUFFER_DESC contantBufferDesc{};
	contantBufferDesc.ByteWidth = sizeof(m_worldViewProjMatrix);
	contantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	contantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	contantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	contantBufferDesc.MiscFlags = 0;
	contantBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData2{};
	initData2.pSysMem = &m_worldViewProjMatrix;
	initData2.SysMemPitch = 0;
	initData2.SysMemSlicePitch = 0;

	// 상수 버퍼 생성
	HR(m_d3dDevice->CreateBuffer(&contantBufferDesc, &initData2, &m_constantBuffer));

	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}

