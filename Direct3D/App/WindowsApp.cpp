// App.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "pch.h"
#include "Application.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
	// 메모리 누수검사하기
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(6941);

    Application* app = new Application();

    constexpr UINT screenWidth = 1200;
	constexpr UINT screenHeight =960;

    app->Initialize(hInstance, nCmdShow, screenWidth,screenHeight);

    app->Process();

    app->Finalize();

    delete app;

    return 0;
}