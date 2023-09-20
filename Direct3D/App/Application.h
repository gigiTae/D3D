#pragma once

/// <summary>
///  모듈들을 이용하는 가장 상단 클래스 
/// </summary>
class Application
{

public:
	Application();
	~Application();

	void Initialize(HINSTANCE hInstance, int nCmdShow, UINT screenWidth, UINT screenHeight );
	void Process();
	void Finalize();

private:
	void WindowInitialize(int nCmdShow);
	void Update();
	void CameraMove();

private:
	UINT m_screenWidth;
	UINT m_screenHeight;

	HINSTANCE m_hInstance;
	HWND m_hWnd; // 메인 윈도우 핸들 

	std::unique_ptr<D3DRenderer> m_d3dRenderer; 
	std::unique_ptr<InputManager> m_inputManager;
	std::unique_ptr<TimeManager> m_timeManager;
};

