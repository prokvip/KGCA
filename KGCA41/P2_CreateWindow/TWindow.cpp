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
        PostQuitMessage(0); // �޼���ť�� ���� WM_QUIT
        break;
    }
    // �޼��� ���� ó�� �Ұ� �ϰ� ��� ����.
    return  DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL TWindow::InitInstance()
{
    // �ü���� ����� �����츦 �����Ѵ�.
    HWND hWnd = CreateWindowW(
        L"KGCA������",
        L"����� �츮���Դϴ�.",
        WS_OVERLAPPEDWINDOW,
        0, 0, 
        800, 600, 
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
    wcex.hbrBackground = CreateSolidBrush(RGB(89, 58, 255));
    wcex.lpszClassName = L"KGCA������";// �̸�
    // ������ �޼����� ���� �Լ��� �����Ѵ�.    
    wcex.lpfnWndProc = WndProc;  // ��ȭ��ȣ  
    wcex.hCursor = LoadCursor(nullptr, IDC_WAIT);
    return RegisterClassEx(&wcex);
}
bool		TWindow::Init()
{   
    // ������ ���
    WORD ret = MyRegisterClass(m_hInstance);
    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance())
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
    MSG msg = { 0, };
    while (WM_QUIT != msg.message)
    {
        // ���� : �޼���ť�� �޼����� ��� ��ȯ��.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg); // �޼��� ����
            DispatchMessage(&msg);  // �޼��� ���ν����� �����Ѵ�.
        }
        else
        {
            Frame();
            Render();
        }
    }
    return true;
}
