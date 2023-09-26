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

/// ����Ʈ
struct DirectionalLight;
struct PointLight;
struct SpotLight;
struct Material;
=======
>>>>>>> parent of ef95daf (축그리드 생성)

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

	/// ����Ʈ �ӽ�
	
	XMMATRIX GetWorldViewProjMatrix();
	CameraObject* GetMainCamera() const { return m_mainCamera.get(); }
private:
	bool InitializeD3D();

	/// ============================== �ӽ� �ڵ� ===============================
	void InitializeMesh();
	void InitializeLight();

	void RenderLight();
	/// ========================================================================
private: 
	int m_screenHeight;
	int m_screenWidth;

	HWND m_hWnd; // ���� ������ �ڵ� 
	bool m_enable4xMass; // 4XMSAA�� ����Ѵٸ� true�� ����

	std::unique_ptr<CameraObject> m_mainCamera; // ���� ī�޶� 

	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

    /// ======================== ���̴� ================================
	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT4X4 m_worldViewProjMatrix;


	ComPtr<ID3D11RasterizerState> m_rasterizerState[2];

	/// ===================== �ӽð�ü ================================
	Box* m_box;
	Grid* m_grid;
	Cylinder* m_cylinder;
	Sphere* m_sphere;
	GeoSphere* m_geoSphere;
<<<<<<< HEAD
	BaseAxis* m_baseAxis;
	Land* m_land;

	/// ======================= ����Ʈ ���� ================================
	DirectionalLight* m_directLight;
	PointLight* m_pointLight;
	SpotLight* m_spotLight;

	Material* m_landMat;
	Material* m_sphereMat;
=======
>>>>>>> parent of ef95daf (축그리드 생성)

};

