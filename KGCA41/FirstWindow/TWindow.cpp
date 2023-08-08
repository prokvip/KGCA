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
    wcex.lpfnWndProc = WndProc; //���������ν��� �����̺�Ʈ->�޼���ť ����
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));// (COLOR_WINDOW + 1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"KGCA������";
    WORD ret = RegisterClassExW(&wcex);
    return true;
}

bool  TWindow::EngineInit()
{
    return true;
}
bool  TWindow::EngineFrame()
{
    return true;
}
bool  TWindow::EngineRender()
{
    return true;
}
bool  TWindow::EngineRelease()
{
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
    m_hWnd = CreateWindowEx(m_dwExStyle, L"KGCA������", szTitle,
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

bool  TWindow::Run()
{
    EngineInit();
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // ���ӷ����� ó��
            EngineFrame();
            EngineRender();
        }
    }
    EngineRelease();
    return true;
}