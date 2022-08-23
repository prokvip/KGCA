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
        PostQuitMessage(0); // 메세지큐에 직접 WM_QUIT
        break;
    }
    // 메세지 내가 처리 불가 니가 대신 해줘.
    return  DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL TWindow::InitInstance(const WCHAR* szTitle, UINT iWidth, UINT iHeight)
{
    // 운영체제에 등록한 윈도우를 생성한다.
    HWND hWnd = CreateWindowW(
        L"KGCA윈도우",
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0, 
        iWidth, iHeight,
        nullptr, nullptr, 
        m_hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, SW_SHOW);
    return TRUE;
}

/// <summary>
///  운영체제에게 윈도우 등록
/// </summary>
/// <param name="hInstance"></param>
/// <returns></returns>
ATOM TWindow::MyRegisterClass()
{
    WNDCLASSEXW  wcex;    
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = m_hInstance;
    wcex.hbrBackground = CreateSolidBrush(RGB(89, 58, 255));
    wcex.lpszClassName = L"KGCA윈도우";// 이름
    // 윈도우 메세지를 받을 함수를 지정한다.    
    wcex.lpfnWndProc = WndProc;  // 전화번호  
    wcex.hCursor = LoadCursor(nullptr, IDC_WAIT);
    return RegisterClassEx(&wcex);
}
bool		TWindow::SetWindow(HINSTANCE hInstance,
                     const WCHAR* szTitle, UINT iWidth, UINT iHeight)
{   
    // 윈도우 등록
    m_hInstance = hInstance;
    WORD ret = MyRegisterClass();
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(szTitle, iWidth, iHeight))
    {
        return FALSE;
    }
    return true;
}
bool		TWindow::Init()
{
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
    Init();

    MSG msg = { 0, };
    while (WM_QUIT != msg.message)
    {
        // 장점 : 메세지큐에 메세지가 없어도 반환됨.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg); // 메세지 번역
            DispatchMessage(&msg);  // 메세지 프로시져에 전달한다.
        }
        else
        {
            Frame();
            Render();
        }
    }
    Release();

    return true;
}
