#include "pch.h"
#include "Application.h"

Application::Application()
	:m_hInstance(nullptr)
	,m_hWnd(nullptr)
	,m_screenSize{0,0}
	,m_d3dRenderer(nullptr)
{

}

Application::~Application()
{

}


void Application::Initialize(HINSTANCE hInstance, int nCmdShow, std::pair<unsigned int, unsigned int> screenSize)
{
	m_hInstance = hInstance;
	m_screenSize = screenSize;

	// ������ �ʱ�ȭ 
	WindowInitialize(nCmdShow);

	// �׷��Ƚ� ���� �ʱ�ȭ 
	m_d3dRenderer = std::make_unique<D3DRenderer>();
	m_d3dRenderer->Initialize(m_hWnd, m_screenSize);

	// ���� ���� �ʱ�ȭ 



}

void Application::Process()
{
	MSG msg;

	while (true)
	{
		// �ȸ޼��� �Լ��� ����ؼ� �޼����� �����Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			// ���� ���μ��� ����
			m_d3dRenderer->Render();
		};

	}
}

void Application::Finalize()
{

	m_d3dRenderer->Finalize();

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Application::WindowInitialize(int nCmdShow)
{
	// ��� ������ �̸�
	constexpr wchar_t title[] = L"Application";

	WNDCLASS wndclass{};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
	wndclass.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = title;

	// ������ Ŭ���� ���
	RegisterClass(&wndclass);

	// ������ â ������ǥ (�»�� ����)
	constexpr int left = 0;
	constexpr int top = 0;


	DWORD dwStyle = WS_OVERLAPPEDWINDOW; // �⺻ ������ ��Ÿ��
	DWORD dwExStyle = 0; // �⺻ Ȯ�� ������ ��Ÿ��

	/// õü ȭ�� ���� ����
	//dwStyle = WS_POPUP; // �˾� ��Ÿ��
	//dwExStyle = WS_EX_TOPMOST; // �ֻ��� ������ ��Ÿ��

	//m_hWnd = CreateWindow(title, title
	//	, dwStyle | dwExStyle // ��Ÿ�ϰ� Ȯ�� ��Ÿ���� �����Ͽ� ���
	//	, left, top, m_screenSize.first, m_screenSize.second
	//	, NULL, NULL, m_hInstance, NULL);

	// ������ â ����
	m_hWnd = CreateWindow(title, title
		, WS_OVERLAPPEDWINDOW // â�� ��ħ
		, left, top, m_screenSize.first, m_screenSize.second
		, NULL, NULL, m_hInstance, NULL);

	// ������ڵ�
	assert(m_hWnd);

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	// ȭ���ػ󵵿� �°� window ��ǥ�踦 �����Ѵ�
	RECT rt = { 0, 0,(long)m_screenSize.first, (long)m_screenSize.second };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false); // window ������Ÿ�Ͽ� �´� rt���� ��ȯ���ش�.
	SetWindowPos(m_hWnd, nullptr, left, top, rt.right - rt.left, rt.bottom - rt.top, 0);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
