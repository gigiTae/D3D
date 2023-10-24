#pragma once



struct SimpleMesh;
class Box;
class Grid;
class Cylinder;
class Sphere;
class GeoSphere;
class BaseAxis;

namespace RendererModule
{
class Crate;
class InputLayout;
class ResourceManager;
class TextManager;
class Camera;
}

namespace RendererModule
{
	/// <summary>
	///  DX11  
	/// 
	/// 
	/// 2023.9.8 23:22 gitae
	/// </summary>
class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();

	void Initialize(HWND hWnd, int screenWidth, int screenHeight);
	void Finalize();
	void ClearScreen();
	void OnResize(int width, int height);
	void Render();
	void BeginRender();
	void EndRender();

	TextManager* GetTextManager() const { return m_textManager.get(); }
	ID3D11Device* GetDevice() const { return m_d3dDevice.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return m_d3dDeviceContext.Get(); }
	Camera* GetCamera() { return m_camera.get(); }

private:
	void InitializeD3D();
	void InitializeResource();
	void InitializeObject();

private:
	int m_screenWidth;
	int m_screenHeight;

	HWND m_hWnd; // 메인 윈도우 핸들 
	bool m_enable4xMass; // 4XMSAA를 사용한다면 true로 설정
	UINT m_4xMsaaQuality;


	std::unique_ptr<ResourceManager> m_resourceManager;
	std::unique_ptr<TextManager> m_textManager;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<InputLayout> m_inputLayout; // 입력서술 배치

	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

#pragma region RenderState
	ComPtr<ID3D11RasterizerState> m_rasterizerState[2];
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

#pragma endregion

		/// ===================== 임시객체 ================================
	Box* m_box;
	Grid* m_grid;
	Cylinder* m_cylinder;
	Sphere* m_sphere;
	GeoSphere* m_geoSphere;
	BaseAxis* m_baseAxis;
	Crate* m_crate;
};
}
