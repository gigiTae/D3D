#include "pch.h"
#include "Application.h"

Application::Application()
	:m_hInstance(nullptr)
	,m_hWnd(nullptr)
	,m_screenHeight(0)
	,m_screenWidth(0)
	,m_d3dRenderer(nullptr)
{

}

Application::~Application()
{

}

void Application::Initialize(HINSTANCE hInstance, int nCmdShow, UINT screenWidth, UINT screenHeight)
{
	m_hInstance = hInstance;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 윈도우 초기화 
	WindowInitialize(nCmdShow);

	// 그래픽스 엔진 초기화 
	m_d3dRenderer = std::make_unique<D3DRenderer>();
	m_d3dRenderer->Initialize(m_hWnd, screenWidth, screenHeight );

	// 매니저 생성
	m_timeManager = std::make_unique<TimeManager>();
	m_inputManager = std::make_unique<InputManager>();

	m_timeManager->Initialize();
	m_inputManager->Initalize(m_hWnd);

}

void Application::Process()
{
	MSG msg;

	while (true)
	{
		// 픽메세지 함수를 사용해서 메세지를 관리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			// 게임 프로세스 루프
			Update();
			m_d3dRenderer->Render();
		};

	}
}

void Application::Update()
{
	m_inputManager->Update();
	m_timeManager->Update();

	CameraMove();
}

void Application::CameraMove()
{
	// 일단 카메라 움직임을 구현해보자
	float deltaTime = m_timeManager->GetDeltaTime();
	float moveSpeed = 10.f;
	float distance = deltaTime * moveSpeed;
	
	if (m_inputManager->IsKeyState(KEY::W, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetMainCamera()->MoveCameraZ(distance);
	}
	if (m_inputManager->IsKeyState(KEY::S, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetMainCamera()->MoveCameraZ(-distance);
	}
	if (m_inputManager->IsKeyState(KEY::A, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetMainCamera()->MoveCameraX(-distance);
	}
	if (m_inputManager->IsKeyState(KEY::D, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetMainCamera()->MoveCameraX(distance);
	}

	if (m_inputManager->IsKeyState(KEY::Q, KEY_STATE::HOLD))
	{
		distance *= 0.5f;
		m_d3dRenderer->GetMainCamera()->RotateCameraY(-distance);
	}
	if (m_inputManager->IsKeyState(KEY::E, KEY_STATE::HOLD))
	{
		distance *= 0.5f;
		m_d3dRenderer->GetMainCamera()->RotateCameraY(distance);
	}
}

void Application::Finalize()
{

	m_d3dRenderer->Finalize();

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Application::WindowInitialize(int nCmdShow)
{
	// 상단 윈도우 이름
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

	// 윈도우 클래스 등록
	RegisterClass(&wndclass);

	// 윈도우 창 시작좌표 (좌상단 기준)
	constexpr int left = 0;
	constexpr int top = 0;


	DWORD dwStyle = WS_OVERLAPPEDWINDOW; // 기본 윈도우 스타일
	DWORD dwExStyle = 0; // 기본 확장 윈도우 스타일

	/// 천체 화면 모드로 변경
	//dwStyle = WS_POPUP; // 팝업 스타일
	//dwExStyle = WS_EX_TOPMOST; // 최상위 윈도우 스타일

	//m_hWnd = CreateWindow(title, title
	//	, dwStyle | dwExStyle // 스타일과 확장 스타일을 결합하여 사용
	//	, left, top, m_screenSize.first, m_screenSize.second
	//	, NULL, NULL, m_hInstance, NULL);

	// 윈도우 창 생성
	m_hWnd = CreateWindow(title, title
		, WS_OVERLAPPEDWINDOW // 창이 겹침
		, left, top, m_screenWidth, m_screenHeight
		, NULL, NULL, m_hInstance, NULL);

	// 방어적코드
	assert(m_hWnd);

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	// 화면해상도에 맞게 window 좌표계를 설정한다
	RECT rt = { 0, 0,(long)m_screenWidth, (long)m_screenHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false); // window 설정스타일에 맞는 rt값을 반환해준다.
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
