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

bool  TWindow::Init() { return true; }
bool  TWindow::Frame() { return true; }
bool  TWindow::Render() { return true; }
bool  TWindow::Release() { return true; }


bool TWindow::SetWindow()
{
    DWORD dwExStyle = 0;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwWindowPosX = 0;
    DWORD dwWindowPosY = 0;
    DWORD dwWindowWidth = 800;
    DWORD dwWindowHeight = 600;

#ifndef _DEBUG
    dwExStyle = WS_EX_TOPMOST;
    dwStyle = WS_POPUPWINDOW;
#else
    dwExStyle = WS_EX_APPWINDOW;
#endif
    m_hWnd = CreateWindowEx(dwExStyle, L"KGCA������",
        L"���� Ÿ��Ʋ", dwStyle,
        dwWindowPosX, dwWindowPosY, dwWindowWidth, dwWindowHeight,
        nullptr, nullptr, m_hInstance, nullptr);
    if (!m_hWnd)
    {
        return FALSE;
    }
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

}

bool  TWindow::Run()
{
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
            Frame();
            Render();
        }
    }
    return true;
}