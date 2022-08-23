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
BOOL TWindow::InitInstance(const WCHAR* szTitle, UINT iWidth, UINT iHeight)
{
    // �ü���� ����� �����츦 �����Ѵ�.
    HWND hWnd = CreateWindowW(
        L"KGCA������",
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
///  �ü������ ������ ���
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
    wcex.lpszClassName = L"KGCA������";// �̸�
    // ������ �޼����� ���� �Լ��� �����Ѵ�.    
    wcex.lpfnWndProc = WndProc;  // ��ȭ��ȣ  
    wcex.hCursor = LoadCursor(nullptr, IDC_WAIT);
    return RegisterClassEx(&wcex);
}
bool		TWindow::SetWindow(HINSTANCE hInstance,
                     const WCHAR* szTitle, UINT iWidth, UINT iHeight)
{   
    // ������ ���
    m_hInstance = hInstance;
    WORD ret = MyRegisterClass();
    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
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
    Release();

    return true;
}
