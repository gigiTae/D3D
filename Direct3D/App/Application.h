#pragma once


/// <summary>
///  ������ �̿��ϴ� ���� ��� Ŭ���� 
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
	HWND m_hWnd; // ���� ������ �ڵ� 

	std::unique_ptr<D3DRenderer> m_d3dRenderer; 


};

