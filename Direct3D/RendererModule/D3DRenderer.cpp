#include "RendererPCH.h"
#include "D3DRenderer.h"
#include "CameraObject.h"

/// Mesh
#include "Box.h"
#include "Cylinder.h"
#include "Grid.h"
#include "Sphere.h"
#include "GeoSphere.h"
<<<<<<< HEAD
#include "BaseAxis.h"
#include "Land.h"

/// Light
#include "Light.h"
=======
>>>>>>> parent of ef95daf (ì¶•ê·¸ë¦¬ë“œ ìƒì„±)

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
	,m_sphere(nullptr)
	,m_cylinder(nullptr)
	,m_geoSphere(nullptr)
<<<<<<< HEAD
	,m_baseAxis(nullptr)
	,m_land(nullptr)
=======
>>>>>>> parent of ef95daf (ì¶•ê·¸ë¦¬ë“œ ìƒì„±)
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

	// Ä«¸Ş¶ó »ı¼º
	m_mainCamera = std::make_unique<CameraObject>();
	XMVECTOR cameraPosition = XMVectorSet(10.f, 20.f, 10.f, 1.f);
	m_mainCamera->Initialize(m_screenWidth, m_screenHeight, cameraPosition);

	InitializeD3D();

	// ¿ùµå 
	XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	// Ä«¸Ş¶ó 
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();

	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();

	// ÃÖÁ¾ Çà·Ä
	XMMATRIX finalMatrix = worldMatrix * viewMatrix * projectMatrix;

	DirectX::XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
	DirectX::XMStoreFloat4x4(&m_worldViewProjMatrix, finalMatrix);
	
<<<<<<< HEAD
	InitializeMesh();
	InitializeLight();
=======
	m_box = new Box(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_box->Initialize();

	m_grid = new Grid(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_grid->Initialize(100, 100, 10, 10);

	m_cylinder = new Cylinder(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_cylinder->Initailize(10.f, 0.f, 10.f, 100, 100);

	m_sphere = new Sphere(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_sphere->Initialize(10.f, 20,20);

	m_geoSphere = new GeoSphere(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_geoSphere->Initilize(10.f, 2);
>>>>>>> parent of ef95daf (ì¶•ê·¸ë¦¬ë“œ ìƒì„±)

	return true;
}

void D3DRenderer::Finalize()
{
	m_mainCamera->Finalize();

	delete m_grid;
	delete m_box;
	delete m_sphere;
	delete m_cylinder;
	delete m_geoSphere;


	delete m_sphereMat;
	delete m_landMat;
	CoUninitialize();
}

void D3DRenderer::ClearScreen()
{
	float arr[4]{ 0.f,0.16f,0.1f,1.f };

	m_d3dDeviceContext->ClearRenderTargetView(m_d3dRenderTargetView.Get(),arr);

	// ±íÀÌ ¹öÆÛ¸¦ 1. Of·Î, ½ºÅÙ½Ç ¹öÆÛ¸¦ 0À¸·Î Áö¿î´Ù.
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void D3DRenderer::Render()
{
	XMFLOAT3 eye;
	XMStoreFloat3(&eye, m_mainCamera->GetPosition());
	
	XMVECTOR dir = m_mainCamera->GetPosition();
	XMVECTOR target = XMVectorZero();

	/// ÀÓ½Ã Update
	XMStoreFloat3(&m_pointLight->position, m_mainCamera->GetPosition());
	XMStoreFloat3(&m_spotLight->direction, XMVector3Normalize(target-dir));
	XMStoreFloat3(&m_spotLight->position, m_mainCamera->GetPosition());

	ClearScreen();



	// ±×¸²À» ±×·Áº¸ÀÚ 
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_worldMatrix);
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();
	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();

	m_box->Update(worldMatrix, viewMatrix, projectMatrix);
	m_box->Render();

	m_grid->Update(worldMatrix, viewMatrix, projectMatrix);
	m_grid->Render();

	m_cylinder->Update(worldMatrix, viewMatrix, projectMatrix);
	m_cylinder->Render();

	m_sphere->Update(worldMatrix, viewMatrix, projectMatrix);
	m_sphere->Render();

	m_geoSphere->Update(worldMatrix, viewMatrix, projectMatrix);
<<<<<<< HEAD
	m_geoSphere->Render(worldMatrix, eye, m_spotLight, m_pointLight, m_directLight);

	m_baseAxis->Update(worldMatrix, viewMatrix, projectMatrix);
	m_baseAxis->Render();

	m_land->Update(worldMatrix, viewMatrix, projectMatrix);
	XMFLOAT3 eyePos;
	XMStoreFloat3(&eyePos, m_mainCamera->GetPosition());

	m_land->Render(m_directLight, m_pointLight, m_spotLight, &eyePos);
=======
	m_geoSphere->Render();
>>>>>>> parent of ef95daf (ì¶•ê·¸ë¦¬ë“œ ìƒì„±)

	HR(m_swapChain->Present(0, 0));
}


DirectX::XMMATRIX D3DRenderer::GetWorldViewProjMatrix()
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_worldMatrix);
	XMMATRIX viewMatrix = m_mainCamera->GetViewMatrix();
	XMMATRIX projectMatrix = m_mainCamera->GetProjectMatrix();
 
	// ÃÖÁ¾ Çà·Ä
	return worldMatrix * viewMatrix * projectMatrix;
}

bool D3DRenderer::InitializeD3D()
{
	/// ==================================
	///          Direct3D ÃÊ±âÈ­ 
	/// ==================================

	// Direct3D ÃÊ±âÈ­ 8°¡Áö ´Ü°è

	// 1. D3D11CreateDevice ÇÔ¼ö¸¦ ÀÌ¿ëÇØ¼­ ÀåÄ¡, Áï ID3D11 Device ÀÎÅÍÆäÀÌ½º¿Í
	// ÀåÄ¡ ¹®¸Æ, Áï ID3D11DeviceContext ÀÎÅÍ ÆäÀÌ½º¸¦ »ı¼ºÇÑ´Ù. 

	UINT createDeviceFlags = 0;

#if defined (DEBUG) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // µğ¹ö±× ¿ëµµ
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

	// 2. ID3D11Device::CheckMultisampleQualityLevels ¸Ş¼­µå¸¦ ÀÌ¿ëÇØ¼­ 
	// 4XMSAA Ç°Áú ¼öÁØ Áö¿ø ¿©ºÎ¸¦ Á¡°ËÇÑ´Ù.

	UINT m4xMsaaQuality{};

	HR(m_d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));

	assert(m4xMsaaQuality > 0);

	// 3. »ı¼ºÇÒ ±³È¯ »ç½½ÀÇ Æ¯¼ºÀ» ¼­¼úÇÏ´Â DXGI_SWAP_CHAIN_DESC ±¸Á¶Ã¼¸¦ Ã¤¿î´Ù.

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = m_screenWidth;
	sd.BufferDesc.Height = m_screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4XMASS¸¦ »ç¿ëÇÏ´Â°¡?
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

	// 4. ÀåÄ¡¸¦ »ı¼ºÇÏ´Âµ¥ »ç¿ëÇß´ø IDXGIFactory ÀÎÅÍÆäÀÌ½º¸¦ ÁúÀÇÇØ¼­ IDXGISwapChain ÀÎ½ºÅÏ½º¸¦ »ı¼ºÇÑ´Ù.

	IDXGIDevice* dxgiDevice = nullptr;
	HR(m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, m_swapChain.GetAddressOf()));

	// COM ÀÎÅÍÆäÀÌ½º ÇØÁ¦
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// 5. ±³È¯ »ç½½ÀÇ ÈÄ¸é ¹öÆÛ¿¡ ´ëÇÑ ·»´õ ´ë»ó ºä¸¦ »ı¼ºÇÑ´Ù.
	ID3D11Texture2D* backBuffer = nullptr;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	assert(backBuffer);

	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, m_d3dRenderTargetView.GetAddressOf()));

	backBuffer->Release();

	// 6. ±íÀÌ, ½ºÅÙ½Ç ¹öÆÛ¿Í ±×¿¡ ¿¬°áµÇ´Â ±íÀÌ,½ºÅÙ½Ç ºä¸¦ »ı¼ºÇÑ´Ù.
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = m_screenWidth;
	depthStencilDesc.Height = m_screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4X MSAA¸¦ »ç¿ëÇÏ´Â°¡ ? ¹İµå½Ã ±³È¯ »ç½½ MSAA ¼³Á¤°ú ÀÏÄ¡ÇØ¾ßÇÔ
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

	// 7. ·£´õ ´ë»ó ºä¿Í ±íÀÌ,½ºÅÙ½Ç ºä¸¦ Direct3D°¡ »ç¿ëÇÒ ¼ö ÀÖµµ·Ï ·»´õ¸µ ÆÄÀÌÇÁ¶óÀÎÀÇ Ãâ·Â º´ÇÕ±â ´Ü°è¿¡ ¹­´Â´Ù.
	m_d3dDeviceContext->OMSetRenderTargets(1, m_d3dRenderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// 8. ºäÆ÷Æ®¸¦ ¼³Á¤ÇÑ´Ù. ºäÆ÷Æ® 2°³¼³Á¤ÇÏ¸é È­¸éºĞÇÒ °¡´É 

	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width = static_cast<float>(m_screenWidth);
	vp.Height = static_cast<float>(m_screenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_d3dDeviceContext->RSSetViewports(1, &vp);

	// ·¡½ºÅÍ¶óÀÌÁî ¼³Á¤

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; // ¿ÍÀÌ¾î ÇÁ·¹ÀÓ ¸ğµå
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID; // »ï°¢Çü Ã¤¿ì±â ¸ğµå 
	//rasterizerDesc.CullMode = D3D11_CULL_BACK; // ÈÄ¸é »ï°¢Çö Á¦¿Ü
	rasterizerDesc.CullMode = D3D11_CULL_NONE;  // Ä«¸Ş¶ó¿¡ ´ëÇÑ ¾ç¸é ·£´õ¸µ È°¼ºÈ­
	rasterizerDesc.FrontCounterClockwise = false; // ½Ã°è ¹æÇâÀ¸·Î ±×·ÁÁö´Â »ï°¢Çü
	rasterizerDesc.DepthClipEnable = true; // ±íÀÌ Å¬¸®ÇÎ È°¼ºÈ­

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_rasterizerState[0].GetAddressOf());

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_rasterizerState[1].GetAddressOf());

	m_d3dDeviceContext->RSSetState(m_rasterizerState[0].Get());

	return true;
}

void D3DRenderer::InitializeMesh()
{
	/// ¾ÆÁ÷±îÁö´Â °ÔÀÓ¿ÀºêÁ§Æ® ¼³°èÇÏ±â´Â ¹«¸®ÀÌ´Ù.  ¾îÂ÷ÇÇ ¾öÃ»³ª°Ô ¶â¾î °íÃÄ¾ßÇÔ
	/// ±×·¯¹Ç·Î °øºÎÇÏÀÚ 
	
	m_box = new Box(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_box->Initialize();

	m_grid = new Grid(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_grid->Initialize(100, 100, 10, 10);

	m_cylinder = new Cylinder(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_cylinder->Initailize(10.f, 0.f, 10.f, 100, 100);

	m_sphere = new Sphere(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_sphere->Initialize(10.f, 20, 20);

	m_geoSphere = new GeoSphere(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[1].Get());
	m_geoSphere->Initilize(5.f, 10);

	m_baseAxis = new BaseAxis(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[0].Get());
	m_baseAxis->Initalize();

	m_land = new Land(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_rasterizerState[1].Get());
	m_land->Initialize(10, 10, 10, 10);

}

void D3DRenderer::InitializeLight()
{
	// Light
	m_directLight = new DirectionalLight();
	m_pointLight = new PointLight;
	m_spotLight = new SpotLight();

	// Directional light.
	m_directLight->ambient  = XMFLOAT4(0.2f, 0.6f, 0.6f, 1.0f);
	m_directLight->diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_directLight->specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_directLight->direction = XMFLOAT3(0.3f, 0.3f, 100.f);

	// Point light--position is changed every frame to animate in UpdateScene function.
	m_pointLight->ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pointLight->diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_pointLight->specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_pointLight->att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	m_pointLight->range = 25.0f;
	m_pointLight->position = XMFLOAT3(0.f, 30.f, 0.f);

	// Spot light--position and direction changed every frame to animate in UpdateScene function.
	m_spotLight->ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_spotLight->diffuse  = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_spotLight->specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_spotLight->att      = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_spotLight->spot     = 96.0f;
	m_spotLight->range = 10000.0f;
	m_spotLight->position = XMFLOAT3(0.f, 30.f, 0.f);

}

void D3DRenderer::RenderLight()
{

}

