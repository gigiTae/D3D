#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                    \
        if(FAILED(hr))                                       \
        {                                                    \
            LPWSTR output;                                   \
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |       \
                FORMAT_MESSAGE_IGNORE_INSERTS      |         \
                FORMAT_MESSAGE_ALLOCATE_BUFFER,              \
                NULL,                                        \
                hr,                                          \
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   \
                (LPTSTR) &output,                            \
                0,                                           \
                NULL);                                       \
            MessageBox(NULL, output, L"Error", MB_OK);       \
		}													 \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

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
	void DrawLine();
	void Finalize();


private: 
	std::pair<unsigned int, unsigned int> m_screenSize;

	HWND m_hWnd; // 메인 윈도우 핸들 

	D3D_FEATURE_LEVEL m_featureLevel;
	

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	bool m_enable4xMass; // 4XMSAA를 사용한다면 trueㄹ호 설정
};

