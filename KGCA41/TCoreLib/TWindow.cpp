#include "TWindow.h"
HWND g_hWnd;
DWORD g_dwClientWidth;
DWORD g_dwClientHeight;
TWindow* g_pWindow = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT hr = g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    if (hr > 0) return 0;

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
int TWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (SIZE_MINIMIZED != wParam)
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            ResizeDevice(width, height);
        }
        break;
    }
    return -1;
}
bool TWindow::SetRegisterClassWindow(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; //윈도우프로시저 각종이벤트->메세지큐 저장
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));// (COLOR_WINDOW + 1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"KGCA윈도우";
    WORD ret = RegisterClassExW(&wcex);
    return true;
}
bool TWindow::SetWindow(const WCHAR* szTitle,//std::wstring szTitle,
    DWORD       dwWindowWidth,
    DWORD       dwWindowHeight)
{   
    
#ifndef _DEBUG
    dwExStyle = WS_EX_TOPMOST;
    dwStyle = WS_POPUPWINDOW;
#else
    m_dwExStyle = WS_EX_APPWINDOW;
#endif
    RECT rc = { 0,0, dwWindowWidth , dwWindowHeight };
    AdjustWindowRect(&rc, m_dwStyle, FALSE);
    m_hWnd = CreateWindowEx(m_dwExStyle, L"KGCA윈도우", szTitle,
        //szTitle.c_str(), 
        m_dwStyle,
        m_dwWindowPosX, m_dwWindowPosY,
        rc.right-rc.left, rc.bottom-rc.top,
        nullptr, nullptr, m_hInstance, nullptr);
    if (!m_hWnd)
    {
        return FALSE;
    }

    g_hWnd = m_hWnd;
   
    GetClientRect(m_hWnd, &m_rcClient);
    g_dwClientWidth = m_dwClientWidth = m_rcClient.right;
    g_dwClientHeight = m_dwClientHeight = m_rcClient.bottom;

    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    return true;
}



TWindow::TWindow()
{
    g_pWindow = this;
}