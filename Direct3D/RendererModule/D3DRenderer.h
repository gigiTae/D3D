#pragma once

struct SimpleMesh;
class CameraObject;
class Box;
class Grid;
class Cylinder;
class Sphere;
class GeoSphere;
<<<<<<< HEAD
class BaseAxis;
class Land;

/// ∂Û¿Ã∆Æ
struct DirectionalLight;
struct PointLight;
struct SpotLight;
struct Material;
=======
>>>>>>> parent of ef95daf (Ï∂ïÍ∑∏Î¶¨Îìú ÏÉùÏÑ±)

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

	bool Initialize(HWND hWnd, int screenWidth, int screenHeight);
	void Finalize();
	void ClearScreen();
	void Render();

	/// ∂Û¿Ã∆Æ ¿”Ω√
	
	XMMATRIX GetWorldViewProjMatrix();
	CameraObject* GetMainCamera() const { return m_mainCamera.get(); }
private:
	bool InitializeD3D();

	/// ============================== ¿”Ω√ ƒ⁄µÂ ===============================
	void InitializeMesh();
	void InitializeLight();

	void RenderLight();
	/// ========================================================================
private: 
	int m_screenHeight;
	int m_screenWidth;

	HWND m_hWnd; // ∏ﬁ¿Œ ¿©µµøÏ «⁄µÈ 
	bool m_enable4xMass; // 4XMSAA∏¶ ªÁøÎ«—¥Ÿ∏È true∑Œ º≥¡§

	std::unique_ptr<CameraObject> m_mainCamera; // ∏ﬁ¿Œ ƒ´∏ﬁ∂Û 

	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

    /// ======================== Ω¶¿Ã¥ı ================================
	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT4X4 m_worldViewProjMatrix;


	ComPtr<ID3D11RasterizerState> m_rasterizerState[2];

	/// ===================== ¿”Ω√∞¥√º ================================
	Box* m_box;
	Grid* m_grid;
	Cylinder* m_cylinder;
	Sphere* m_sphere;
	GeoSphere* m_geoSphere;
<<<<<<< HEAD
	BaseAxis* m_baseAxis;
	Land* m_land;

	/// ======================= ∂Û¿Ã∆Æ ∞¸∑√ ================================
	DirectionalLight* m_directLight;
	PointLight* m_pointLight;
	SpotLight* m_spotLight;

	Material* m_landMat;
	Material* m_sphereMat;
=======
>>>>>>> parent of ef95daf (Ï∂ïÍ∑∏Î¶¨Îìú ÏÉùÏÑ±)

};

