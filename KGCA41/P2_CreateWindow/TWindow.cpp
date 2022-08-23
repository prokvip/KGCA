#include "TWindow.h"
LRESULT CALLBACK WndProc(
    HWND hWnd, 
    UINT message, 
    WPARAM wParam, 
    LPARAM lParam)
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
BOOL TWindow::InitInstance(
    HINSTANCE hInstance)
{
    // 운영체제에 등록한 윈도우를 생성한다.
    HWND hWnd = CreateWindowW(
        L"홍길동윈도우",
        L"여기는 우리집입니다.",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 
        CW_USEDEFAULT, 0, 
        nullptr, nullptr, 
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOW);
    //UpdateWindow(hWnd);
    return TRUE;
}

/// <summary>
///  운영체제에게 윈도우 등록
/// </summary>
/// <param name="hInstance"></param>
/// <returns></returns>
ATOM TWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW  wcex;    
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 
    wcex.lpszClassName = L"홍길동윈도우";
    // 윈도우 메세지를 받을 함수를 지정한다.    
    wcex.lpfnWndProc = WndProc;    
    
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CREATEWINDOW));
    //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CREATEWINDOW);
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassEx(&wcex);
}
bool		TWindow::Init()
{   
    // 윈도우 등록
    WORD ret = MyRegisterClass(m_hInstance);
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(m_hInstance))
    {
        return FALSE;
    }
    return true;
}
bool		TWindow::Frame()
{
    return true;
}
bool		TWindow::Render()
{
    return true;
}
bool		TWindow::Release()
{
    return true;
}

bool        TWindow::Run()
{
    MSG msg;
    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}
