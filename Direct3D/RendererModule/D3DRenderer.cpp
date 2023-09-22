#include "RendererPCH.h"
#include "D3DRenderer.h"
#include "CameraObject.h"
#include "Box.h"
#include "Cylinder.h"
#include "Grid.h"

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
	, m_screenWidth(0)
	, m_screenHeight(0)
	,m_box(nullptr)
	,m_grid(nullptr)
{
	
}

D3DRenderer::~D3DRenderer()
{

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

	// 월드 
	XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	// 카메라 
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();

	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();

	// 최종 행렬
	XMMATRIX finalMatrix = worldMatrix * viewMatrix * projectMatrix;

	DirectX::XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
	DirectX::XMStoreFloat4x4(&m_worldViewProjMatrix, finalMatrix);
	
	m_box = new Box(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_box->Initialize();

	m_grid = new Grid(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_grid->Initialize(100, 100, 10, 10);

	m_cylinder = new Cylinder(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_cylinder->Initailize(10.f, 10.f, 10.f, 100, 100);

	return true;
}

void D3DRenderer::Finalize()
{
	m_mainCamera->Finalize();

	delete m_grid;
	delete m_box;

	CoUninitialize();
}

void D3DRenderer::ClearScreen()
{
	float arr[4]{ 0.f,0.16f,0.1f,1.f };

	m_d3dDeviceContext->ClearRenderTargetView(m_d3dRenderTargetView.Get(),arr);

	// 깊이 버퍼를 1. Of로, 스텐실 버퍼를 0으로 지운다.
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void D3DRenderer::Render()
{
	ClearScreen();

	// 그림을 그려보자 
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_worldMatrix);
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();
	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();

	m_box->Update(worldMatrix, viewMatrix, projectMatrix);
	m_box->Render();

	m_grid->Update(worldMatrix, viewMatrix, projectMatrix);
	m_grid->Render();

	m_cylinder->Update(worldMatrix, viewMatrix, projectMatrix);
	m_cylinder->Render();

	HR(m_swapChain->Present(0, 0));
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
		m_d3dDevice.GetAddressOf(),
		&m_featureLevel,
		m_d3dDeviceContext.GetAddressOf()
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

	HR(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, m_swapChain.GetAddressOf()));

	// COM 인터페이스 해제
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// 5. 교환 사슬의 후면 버퍼에 대한 렌더 대상 뷰를 생성한다.
	ID3D11Texture2D* backBuffer = nullptr;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	assert(backBuffer);

	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, m_d3dRenderTargetView.GetAddressOf()));

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

	HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, m_depthStencilBuffer.GetAddressOf()));

	assert(m_depthStencilBuffer);
	HR(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView));

	// 7. 랜더 대상 뷰와 깊이,스텐실 뷰를 Direct3D가 사용할 수 있도록 렌더링 파이프라인의 출력 병합기 단계에 묶는다.
	m_d3dDeviceContext->OMSetRenderTargets(1, m_d3dRenderTargetView.GetAddressOf(), m_depthStencilView.Get());

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

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_rasterizerState[0].GetAddressOf());

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_rasterizerState[1].GetAddressOf());

	m_d3dDeviceContext->RSSetState(m_rasterizerState[0].Get());

	return true;
}

