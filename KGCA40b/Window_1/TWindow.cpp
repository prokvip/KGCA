#include "TWindow.h"
LRESULT  CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);// WM_QUIT
    }break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

BOOL TWindow::WinRegisterClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    // 1, 윈도우 클래스 등록, 허가(운영체제)
    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(WNDCLASSW));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"KGCA_Window";
    if (RegisterClass(&wc) == false)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL TWindow::SetWindow(const WCHAR* szTitle, 
    int iClientWidth,
    int iClientHeight)
{
    RECT rt = { 0,0, iClientWidth , iClientHeight };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
    // 2, 윈도우 생성
    m_hWnd = CreateWindow(
        L"KGCA_Window",
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rt.right-rt.left, 
        rt.bottom-rt.top, 
        NULL,
        NULL,
        m_hInstance,
        NULL);
    if (m_hWnd == NULL)
    {
        return FALSE;
    }
    GetClientRect(m_hWnd, &m_rtClient);
    GetWindowRect(m_hWnd, &m_rtWindow);

    ShowWindow(m_hWnd, SW_SHOW);
    return TRUE;
}

bool TWindow::WinRun()
{
    MSG msg;
    // 메세지큐에 메세지가 없으면 블록
    while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);// 메세지 번역
        DispatchMessage(&msg);// 윈도우프로시져 호출
    }
    return true;
}
