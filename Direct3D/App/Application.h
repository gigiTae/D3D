#pragma once


/// <summary>
///  모듈들을 이용하는 가장 상단 클래스 
/// </summary>
class Application
{
public:
	Application();
	~Application();

	void Initialize(HINSTANCE hInstance, int nCmdShow, std::pair<unsigned int, unsigned int> screenSize); 
	void Process();
	void Finalize();

private:
	void WindowInitialize(int nCmdShow);

private:
	std::pair<unsigned int, unsigned int> m_screenSize;

	HINSTANCE m_hInstance;
	HWND m_hWnd; // 메인 윈도우 핸들 

	std::unique_ptr<D3DRenderer> m_d3dRenderer; 


};

