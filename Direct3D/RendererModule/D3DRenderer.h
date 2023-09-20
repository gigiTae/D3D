#pragma once

struct SimpleMesh;
class CameraObject;

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
	void ClearBuffer();
	void Render();
	
	// ����
	void DrawBox();

	DirectX::XMMATRIX GetWorldViewProjMatrix();
	CameraObject* GetMainCamera() const { return m_mainCamera.get(); }
private:
	bool InitializeD3D();
	bool InitializePipeLine();
	void CreateGrid(float width, float depth, UINT m, UINT n, SimpleMesh& mesh);

private: 
	int m_screenHeight;
	int m_screenWidth;

	HWND m_hWnd; // ���� ������ �ڵ� 
	bool m_enable4xMass; // 4XMSAA�� ����Ѵٸ� true�� ����

	std::unique_ptr<CameraObject> m_mainCamera; // ���� ī�޶� 

	D3D_FEATURE_LEVEL m_featureLevel;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout[2];

    /// ======================== ���̴� ================================
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	DirectX::XMFLOAT4X4 m_worldMatrix;
	DirectX::XMFLOAT4X4 m_worldViewProjMatrix;

	/// ======================== ���� ===================================
	ID3D11Buffer* m_constantBuffer;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	SimpleMesh* m_mesh;
};

