#include <windows.h>
// �޼��� ��� ó��
// �ü��-> �޼��� ����(����)->�޼��� ť ����->WndProc ȣ��
// �ü��-> �޼��� ����(����)->WndProc ȣ��
// �������� ���� �޼��� ó�� �Լ�(��ƾ)
// 
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(   HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPWSTR    lpCmdLine,
                         int       nCmdShow)
{
    // 1, ������ Ŭ���� ���, �㰡(�ü��)
    WNDCLASSW wc;    
    ZeroMemory(&wc, sizeof(WNDCLASSW));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc= WndProc;
    wc.hInstance = hInstance;    
    wc.lpszClassName = L"KGCA_Window";
    if (RegisterClass(&wc) == false)
    {
        return 0;
    }
    // 2, ������ ����
    HWND hWnd = CreateWindow(
        L"KGCA_Window",
        L"kgca",
        WS_OVERLAPPEDWINDOW,
        200, 200,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (hWnd == NULL)
    {
        return 0;
    }
    ShowWindow(hWnd, SW_SHOW);

    MSG msg;
    // �⺻ �޽��� �����Դϴ�:
    while (1)
    {
        // �޼���ť�� �޼����� ������ ���
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);// �޼��� ����
            DispatchMessage(&msg);// ���������ν��� ȣ��
        }
        else
        {
            // ���ӷ��� ȣ��
            static int count = 0;
            count++;
        }    
      
    }

    //MessageBox(NULL, L"���� ���� �� ����", L"kgca", MB_YESNOCANCEL);
    return 1;
}
LRESULT  CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);// WM_QUIT
        }break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}