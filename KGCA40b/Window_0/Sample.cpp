#include <windows.h>
// 메세지 기반 처리
// 운영체제-> 메세지 전달(통지)->메세지 큐 저장->WndProc 호출
// 운영체제-> 메세지 전달(통지)->WndProc 호출
// 윈도우의 각종 메세지 처리 함수(루틴)
// 
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(   HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPWSTR    lpCmdLine,
                         int       nCmdShow)
{
    // 1, 윈도우 클래스 등록, 허가(운영체제)
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
    // 2, 윈도우 생성
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
    // 기본 메시지 루프입니다:
    while (1)
    {
        // 메세지큐에 메세지가 없으면 블록
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);// 메세지 번역
            DispatchMessage(&msg);// 윈도우프로시져 호출
        }
        else
        {
            // 게임로직 호출
            static int count = 0;
            count++;
        }    
      
    }

    //MessageBox(NULL, L"정말 종료 할 꺼야", L"kgca", MB_YESNOCANCEL);
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