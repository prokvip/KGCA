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
    // �ü���� ����� �����츦 �����Ѵ�.
    HWND hWnd = CreateWindowW(
        L"ȫ�浿������",
        L"����� �츮���Դϴ�.",
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
///  �ü������ ������ ���
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
    wcex.lpszClassName = L"ȫ�浿������";
    // ������ �޼����� ���� �Լ��� �����Ѵ�.    
    wcex.lpfnWndProc = WndProc;    
    
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CREATEWINDOW));
    //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CREATEWINDOW);
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassEx(&wcex);
}
bool		TWindow::Init()
{   
    // ������ ���
    WORD ret = MyRegisterClass(m_hInstance);
    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
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
    // �⺻ �޽��� �����Դϴ�:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}
