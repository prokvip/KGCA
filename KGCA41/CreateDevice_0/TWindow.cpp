#include "TWindow.h"
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
    m_dwWindowWidth = dwWindowWidth;
    m_dwWindowHeight = dwWindowHeight;
#ifndef _DEBUG
    dwExStyle = WS_EX_TOPMOST;
    dwStyle = WS_POPUPWINDOW;
#else
    m_dwExStyle = WS_EX_APPWINDOW;
#endif
    m_hWnd = CreateWindowEx(m_dwExStyle, L"KGCA윈도우", szTitle,
        //szTitle.c_str(), 
        m_dwStyle,
        m_dwWindowPosX, m_dwWindowPosY, 
        m_dwWindowWidth, m_dwWindowHeight,
        nullptr, nullptr, m_hInstance, nullptr);
    if (!m_hWnd)
    {
        return FALSE;
    }
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    return true;
}
