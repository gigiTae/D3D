#include "RendererPCH.h"
#include "D3DRenderer.h"

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
{
}

D3DRenderer::~D3DRenderer()
{

}


bool D3DRenderer::Initialize(HWND hWnd, std::pair<unsigned int, unsigned int> screenSize)
{
	m_screenSize = screenSize;
	m_hWnd = hWnd;

	InitializeD3D();
	InitializePipeLine();



	return true;
}

void D3DRenderer::Finalize()
{



	CoUninitialize();
}

void D3DRenderer::ClearBuffer()
{
	float arr[4]{ 0.f,0.f,0.f,1.f };

	// �ĸ� ���۸��Ķ������������. Colors: :Blue�� d3dUtil.h�� ���ǵǾ� �ִ�.
	m_d3dDeviceContext->ClearRenderTargetView(m_d3dRenderTargetView.Get(),
		arr);
	// ���� ���۸� 1. Of��, ���ٽ� ���۸� 0���� �����.
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void D3DRenderer::Render()
{
	ClearBuffer();

	//// ���� ����
	//DM::Vertex1 vertices[] = {
	//	DirectX::XMFLOAT3(-0.5f, 0.f,0.f), DirectX::XMFLOAT4(0.5f,0.5f,255.f,1.f),
	//	DirectX::XMFLOAT3(-0.5f, 0.5f, 0.f), DirectX::XMFLOAT4(0.3f,0.2f,1.f,1.f),
	//	DirectX::XMFLOAT3(0.f, -0.5f, 1.f), DirectX::XMFLOAT4(0.f,0.3f,0.5f,1.f)
	//};
	// ���� ����
	
	//D3D11_BUFFER_DESC BF{};
	//BF.ByteWidth = sizeof(DM::Vertex1) * 8; // ������ ���� ������ ũ��
	//BF.Usage = D3D11_USAGE_DEFAULT;  // ���۰� ���̴� ��� 
	//BF.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���� ����
	//BF.CPUAccessFlags = 0;
	//BF.MiscFlags = 0;
	//BF.StructureByteStride = 0;

	//D3D11_SUBRESOURCE_DATA SD{};
	//SD.pSysMem = &vertices;

	//ID3D11Buffer* mVB;

	//HR(m_d3dDevice->CreateBuffer(&BF, &SD, &mVB));

	//// ������ ������ �������� ������ �������������� �����Ϸ���, ���� ��ġ��
	//// �� �Է� ���Կ� �ٿ��� �Ѵ�. 

	//UINT stride = sizeof(DM::Vertex1);
	//UINT offset = 0;

	//m_d3dDeviceContext->IASetVertexBuffers(
	//	0, 1, &mVB, &stride, &offset);

	//// ���ΰ� ���� ���� ����
	//UINT indices[6]
	//{
	//	0,1,
	//	0,2,
	//	1,2,
	//};

	//// ���� ���۸� �����ϴ� ����ü�� ä���.
	//D3D11_BUFFER_DESC ibd{};
	//ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(UINT) * 6;
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//ibd.StructureByteStride = 0;

	//// ���� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�.
	//D3D11_SUBRESOURCE_DATA initData{};
	//initData.pSysMem = indices;

	//// ���� ���۸� �����Ѵ�.
	//ID3D11Buffer* mIB;
	//HR(m_d3dDevice->CreateBuffer(&ibd, &initData, &mIB));

	//// ������ ���ο� �����Ѵ�. 
	//m_d3dDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);


	//m_d3dDeviceContext->DrawIndexed(6, 0, 0);

	DrawBox();
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	HR(m_swapChain->Present(0, 0));
}

void D3DRenderer::DrawBox()
{
	using namespace DirectX;

	static int i = 0;
	if (i == 0)
	{


		/// ���ؽ� ����

		XMFLOAT4 green = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		XMFLOAT4 blue = XMFLOAT4(0.f, 0.f, 1.f, 1.f);


		DM::Vertex1 vertices[] =
		{
			// ���� 
			XMFLOAT3(-2.f, -2.f,-2.f), green,
			XMFLOAT3(-2.f, 2.f, -2.f), blue,
			XMFLOAT3(2.f, -2.f, -2.f), green,
			XMFLOAT3(2.f, 2.f, -2.f), blue,

			// �ĸ�
			XMFLOAT3(-2.f, -2.f,2.f), green,
			XMFLOAT3(-2.f, 2.f, 2.f), blue,
			XMFLOAT3(2.f, -2.f, 2.f), green,
			XMFLOAT3(2.f, 2.f, 2.f), blue,
		};

		D3D11_BUFFER_DESC BF{};
		BF.ByteWidth = sizeof(DM::Vertex1) * 8; // ������ ���� ������ ũ��
		BF.Usage = D3D11_USAGE_DEFAULT;  // ���۰� ���̴� ��� 
		BF.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���� ����
		BF.CPUAccessFlags = 0;
		BF.MiscFlags = 0;
		BF.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = &vertices;

		ID3D11Buffer* mVB;

		HR(m_d3dDevice->CreateBuffer(&BF, &SD, &mVB));

		UINT stride = sizeof(DM::Vertex1);
		UINT offset = 0;

		m_d3dDeviceContext->IASetVertexBuffers(
			0, 1, &mVB, &stride, &offset);

		/// �ε��� ����

		// ���ΰ� ���� ���� ����
		UINT indices[24]
		{
			0,1,
			0,2,
			1,3,
			2,3,
			
			0,4,
			2,6,
			1,5,
			3,7,

			4,6,
			4,5,
			6,7,
			7,5

		};

		// ���� ���۸� �����ϴ� ����ü�� ä���.
		D3D11_BUFFER_DESC ibd{};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 24;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		// ���� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�.
		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = indices;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch =0;

		// ���� ���۸� �����Ѵ�.
		ID3D11Buffer* mIB;
		HR(m_d3dDevice->CreateBuffer(&ibd, &initData, &mIB));

		// ������ ���ο� �����Ѵ�. 
		m_d3dDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		/// ��� ����

		// ���� 
		XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		worldMatrix = worldMatrix;

		// ī�޶� 
		XMVECTOR eyePos = XMVectorSet(0.f,0.f,5.f,1.f);
		XMVECTOR focusPos = XMVectorZero();
		XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		XMMATRIX cameraMatrix = XMMatrixLookAtLH(eyePos, focusPos, up);
		
		// ���� ���
		float aspectRaito = static_cast<float>(m_screenSize.first) / static_cast<float>(m_screenSize.second);
		XMMATRIX projectMatrix = XMMatrixPerspectiveFovLH(3.14* 0.25f, aspectRaito, 1.f, 1000.f);
		// ���� ���
		XMMATRIX finalMatrix = worldMatrix * cameraMatrix * projectMatrix;

		DirectX::XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
		DirectX::XMStoreFloat4x4(&m_viewMatrix, cameraMatrix);
		DirectX::XMStoreFloat4x4(&m_projMatrix, projectMatrix);
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

		// ��� ���� ����
		HR(m_d3dDevice->CreateBuffer(&contantBufferDesc, &initData2, &m_constantBuffer));

		m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
 
		++i;
	};

	XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	static float r = 0.f;
	r += 0.0001f;
	worldMatrix = worldMatrix * XMMatrixRotationY(r);

	// ī�޶� 
	XMVECTOR eyePos = XMVectorSet(5, 10.f, -20.f, 1.f);
	XMVECTOR focusPos = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMMATRIX cameraMatrix = XMMatrixLookAtLH(eyePos, focusPos, up);

	// ���� ���
	float aspectRaito = static_cast<float>(m_screenSize.first) / static_cast<float>(m_screenSize.second);
	XMMATRIX projectMatrix = XMMatrixPerspectiveFovLH(3.1415f * 0.25f, aspectRaito, 1.f, 1000.f);

	// ���� ���
	XMMATRIX finalMatrix =  worldMatrix * cameraMatrix * projectMatrix;
	
	/// ������ۿ����� ��ġ����� �����־���Ѵ�!!!!!!!!!!!
	finalMatrix = XMMatrixTranspose(finalMatrix);

	DirectX::XMStoreFloat4x4(&m_worldViewProjMatrix, finalMatrix);

	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	//
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_d3dDeviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// ��� ������ �����͸� �����ɴϴ�.
    XMFLOAT4X4* dataPtr = (XMFLOAT4X4*)mappedResource.pData;

	// ��� ���ۿ� ������Ʈ�� �����͸� �����մϴ�.
	*dataPtr = m_worldViewProjMatrix;

	// ��� ���۸� ����մϴ�.
	m_d3dDeviceContext->Unmap(m_constantBuffer, 0);


	m_d3dDeviceContext->DrawIndexed(24, 0, 0);
}

bool D3DRenderer::InitializeD3D()
{
	/// ==================================
	///          Direct3D �ʱ�ȭ 
	/// ==================================

	// Direct3D �ʱ�ȭ 8���� �ܰ�

	// 1. D3D11CreateDevice �Լ��� �̿��ؼ� ��ġ, �� ID3D11 Device �������̽���
	// ��ġ ����, �� ID3D11DeviceContext ���� ���̽��� �����Ѵ�. 

	UINT createDeviceFlags = 0;

#if defined (DEBUG) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // ����� �뵵
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

	// 2. ID3D11Device::CheckMultisampleQualityLevels �޼��带 �̿��ؼ� 
	// 4XMSAA ǰ�� ���� ���� ���θ� �����Ѵ�.

	UINT m4xMsaaQuality{};

	HR(m_d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));

	assert(m4xMsaaQuality > 0);

	// 3. ������ ��ȯ �罽�� Ư���� �����ϴ� DXGI_SWAP_CHAIN_DESC ����ü�� ä���.

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = m_screenSize.first;
	sd.BufferDesc.Height = m_screenSize.second;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4XMASS�� ����ϴ°�?
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

	// 4. ��ġ�� �����ϴµ� ����ߴ� IDXGIFactory �������̽��� �����ؼ� IDXGISwapChain �ν��Ͻ��� �����Ѵ�.

	IDXGIDevice* dxgiDevice = nullptr;
	HR(m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, &m_swapChain));

	// COM �������̽� ����
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// 5. ��ȯ �罽�� �ĸ� ���ۿ� ���� ���� ��� �並 �����Ѵ�.
	ID3D11Texture2D* backBuffer = nullptr;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	assert(backBuffer);

	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_d3dRenderTargetView));

	backBuffer->Release();

	// 6. ����, ���ٽ� ���ۿ� �׿� ����Ǵ� ����,���ٽ� �並 �����Ѵ�.
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = m_screenSize.first;
	depthStencilDesc.Height = m_screenSize.second;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4X MSAA�� ����ϴ°� ? �ݵ�� ��ȯ �罽 MSAA ������ ��ġ�ؾ���
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

	// 7. ���� ��� ��� ����,���ٽ� �並 Direct3D�� ����� �� �ֵ��� ������ ������������ ��� ���ձ� �ܰ迡 ���´�.

	/// ComPtr�����ΰ� ? OMSetRenderTargets�� �Լ� ȣ��ڿ� NULL�� ���� ???? ������ ���ϳ� COMPTR ���θ� ���ؾ��ҵ�
	ID3D11RenderTargetView* renderTargetView = m_d3dRenderTargetView.Get();

	m_d3dDeviceContext->OMSetRenderTargets(1, &renderTargetView, m_depthStencilView.Get());

	// 8. ����Ʈ�� �����Ѵ�. ����Ʈ 2�������ϸ� ȭ����� ���� 

	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width = static_cast<float>(m_screenSize.first);
	vp.Height = static_cast<float>(m_screenSize.second);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_d3dDeviceContext->RSSetViewports(1, &vp);

	return true;
}

bool D3DRenderer::InitializePipeLine()
{
	HRESULT hr = S_OK;

	ID3D10Blob* vertexShader = nullptr;
	ID3D10Blob* pixelShader = nullptr;

	std::wstring vertexShaderPath = L"C:\\Users\\User\\Desktop\\D3D\\D3D\\Direct3D\\App\\VertexShader.hlsl";

	// ���̴� �ҷ�����
	HR(D3DCompileFromFile(L"VertexShader.hlsl", 0, 0
		, "main", "vs_5_0", 0, 0, &vertexShader, 0));

	HR(D3DCompileFromFile(L"PixelShader.hlsl", 0, 0
		, "main", "ps_5_0", 0, 0, &pixelShader, 0));

	// ���̴� ����
	HR(m_d3dDevice->CreateVertexShader(
		vertexShader->GetBufferPointer(),
		vertexShader->GetBufferSize(),
		NULL, &m_vertexShader));

	HR(m_d3dDevice->CreatePixelShader(
		pixelShader->GetBufferPointer(),
		pixelShader->GetBufferSize(),
		NULL, &m_pixelShader));

	// ���̴� ����
	m_d3dDeviceContext->VSSetShader(m_vertexShader.Get(), 0, 0);
	m_d3dDeviceContext->PSSetShader(m_pixelShader.Get(), 0, 0);

	// �Է� ���̾ƿ� ��ü ����
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HR(m_d3dDevice->CreateInputLayout(ied, 2, vertexShader->GetBufferPointer()
		, vertexShader->GetBufferSize(), &m_inputLayout[0]));

	m_d3dDeviceContext->IASetInputLayout(m_inputLayout[0].Get());

	//HR(m_d3dDevice->CreateInputLayout(ied, 2, pixelShader->GetBufferPointer()
	//	, pixelShader->GetBufferSize(), &m_inputLayout[1]));

	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
