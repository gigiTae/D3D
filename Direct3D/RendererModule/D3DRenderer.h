#pragma once


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

	bool Initialize(HWND hWnd, std::pair<unsigned int, unsigned int> screenSize);
	void Finalize();
	void ClearBuffer();
	void Render();
	
	// 연습
	void DrawBox();


private:
	bool InitializeD3D();
	bool InitializePipeLine();

private: 
	std::pair<unsigned int, unsigned int> m_screenSize;

	HWND m_hWnd; // 메인 윈도우 핸들 
	bool m_enable4xMass; // 4XMSAA를 사용한다면 true로 설정

	D3D_FEATURE_LEVEL m_featureLevel;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout[2];

    /// ======================== 쉐이더 ================================
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	DirectX::XMFLOAT4X4 m_worldMatrix;
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projMatrix;
	DirectX::XMFLOAT4X4 m_worldViewProjMatrix;

	ID3D11Buffer* m_constantBuffer;
	/// ======================== 버퍼 ===================================
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;


};

