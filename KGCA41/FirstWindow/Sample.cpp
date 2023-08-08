#include <windows.h>

bool  Init() { return true; }
bool  Frame() { return true; }
bool  Render() { return true; }
bool  Release() { return true; }


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
int APIENTRY wWinMain( HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPWSTR    lpCmdLine,  int       nCmdShow)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; //윈도우프로시저 각종이벤트->메세지큐 저장
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255,0,0));// (COLOR_WINDOW + 1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"KGCA윈도우";
    WORD ret = RegisterClassExW(&wcex);  

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
    HWND hWnd = CreateWindowEx(dwExStyle, L"KGCA윈도우",
                              L"샘플 타이틀", dwStyle,
                              dwWindowPosX, dwWindowPosY, dwWindowWidth, dwWindowHeight, 
                              nullptr,   nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, SW_SHOWNORMAL);


    Init();

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
            // 게임로직을 처리
            Frame();
            Render();
        }
    }

    Release();
    //::MessageBox(NULL, L"종료", L"타이틀", MB_OK);
    return 0;
}