#include "pch.h"
#include "Application.h"

static UINT g_resizeWidth = 0, g_resizeHeight = 0;

Application::Application()
	:m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_screenHeight(0)
	, m_screenWidth(0)
	, m_d3dRenderer(nullptr)
	, m_inputManager(nullptr)
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

	// ������ �ʱ�ȭ 
	WindowInitialize(nCmdShow);

	// �׷��Ƚ� ���� �ʱ�ȭ 
	m_d3dRenderer = std::make_unique<RendererModule::D3DRenderer>();
	m_d3dRenderer->Initialize(m_hWnd, m_screenWidth, m_screenHeight);

	/// tool
	m_imguiManager = std::make_unique<ToolModule::ImGuiManager>();
	m_imguiManager->Initialize(m_hWnd, m_d3dRenderer->GetDevice(), m_d3dRenderer->GetDeviceContext());


	// �Ŵ��� ����
	m_timeManager = std::make_unique<EngineModule::TimeManager>();
	m_inputManager = std::make_unique<EngineModule::InputManager>();

	m_timeManager->Initialize();
	m_inputManager->Initalize(m_hWnd);

}

void Application::Finalize()
{
	m_imguiManager->Finalize();
	m_d3dRenderer->Finalize();
}

void Application::Process()
{
	MSG msg;

	bool isDone = false;
	while (!isDone)
	{
		// �ȸ޼��� �Լ��� ����ؼ� �޼����� �����Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				isDone = true;
				break;
			}
			DispatchMessage(&msg);
		}
		else
		{
			if (g_resizeHeight != 0 && g_resizeWidth != 0)
			{
				m_d3dRenderer->OnResize(g_resizeWidth, g_resizeHeight);
				g_resizeHeight = 0;
				g_resizeWidth = 0;
			}

			// ���� ���μ��� ����
			Update();


			float dt = m_timeManager->GetDeltaTime();
			int fps = m_timeManager->GetFPS();

			std::wstring DT = L"DT : " + std::to_wstring(dt) + L" FPS : " + std::to_wstring(fps);

			m_d3dRenderer->GetTextManager()->DrawTextColor(XMFLOAT2(0.f, 25.f), XMFLOAT4(0.f, 1.f, 1.f, 1.f), DT);

			m_imguiManager->NewFrame();

			float s = 1.f;
			if (ImGui::Begin("camera"))
			{
				XMFLOAT3 pos = m_d3dRenderer->GetCamera()->GetPosition();

				ImGui::SliderFloat(" X", &pos.x, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat(" Y", &pos.y, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat(" Z", &pos.z, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

				m_d3dRenderer->GetCamera()->SetPosition(pos);

				ImGui::End();
			}

			m_d3dRenderer->BeginRender();
			m_d3dRenderer->Render();


			// imgui
			m_imguiManager->EndRnder();
			m_d3dRenderer->EndRender();

		}
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

	// �ϴ� ī�޶� �������� �����غ���
	float deltaTime = m_timeManager->GetDeltaTime();
	float moveSpeed = 10.f;
	float distance = deltaTime * moveSpeed;

	if (m_inputManager->IsKeyState(KEY::W, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetCamera()->Walk(distance);
	}
	if (m_inputManager->IsKeyState(KEY::S, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetCamera()->Walk(-distance);
	}
	if (m_inputManager->IsKeyState(KEY::A, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetCamera()->Strafe(-distance);
	}
	if (m_inputManager->IsKeyState(KEY::D, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetCamera()->Strafe(distance);
	}
	if (m_inputManager->IsKeyState(KEY::Q, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetCamera()->WorldUpDown(distance);
	}
	if (m_inputManager->IsKeyState(KEY::E, KEY_STATE::HOLD))
	{
		m_d3dRenderer->GetCamera()->WorldUpDown(-distance);
	}


	if (m_inputManager->IsKeyState(KEY::LEFT, KEY_STATE::HOLD))
	{
		distance *= 0.2f;
		m_d3dRenderer->GetCamera()->RotateY(-distance);
	}
	if (m_inputManager->IsKeyState(KEY::RIGHT, KEY_STATE::HOLD))
	{
		distance *= 0.2f;
		m_d3dRenderer->GetCamera()->RotateY(distance);
	}
	if (m_inputManager->IsKeyState(KEY::DOWN, KEY_STATE::HOLD))
	{
		distance *= 0.2f;
		m_d3dRenderer->GetCamera()->Pitch(distance);
	}
	if (m_inputManager->IsKeyState(KEY::UP, KEY_STATE::HOLD))
	{
		distance *= 0.2f;
		m_d3dRenderer->GetCamera()->Pitch(-distance);
	}

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
						  , left, top, m_screenWidth, m_screenHeight
						  , NULL, NULL, m_hInstance, NULL);

					  // ������ڵ�
	assert(m_hWnd);

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	// ȭ���ػ󵵿� �°� window ��ǥ�踦 �����Ѵ�
	RECT rt = { 0, 0,(long)m_screenWidth, (long)m_screenHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false); // window ������Ÿ�Ͽ� �´� rt���� ��ȯ���ش�.
	SetWindowPos(m_hWnd, nullptr, left, top, rt.right - rt.left, rt.bottom - rt.top, 0);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
	PostQuitMessage(0);
	break;
	case WM_SIZE:
	if (wParam == SIZE_MINIMIZED)
		return 0;
	g_resizeWidth = (UINT)LOWORD(lParam); // Queue resize
	g_resizeHeight = (UINT)HIWORD(lParam);
	return 0;
	case WM_DPICHANGED:
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
	{
		//const int dpi = HIWORD(wParam);
		//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
		const RECT* suggested_rect = (RECT*)lParam;
		::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	break;
	default:
	return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
