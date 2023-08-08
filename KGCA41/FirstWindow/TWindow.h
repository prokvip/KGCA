#pragma once
#include <windows.h>
class TWindow
{
    HINSTANCE   m_hInstance;
    HWND        m_hWnd;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
    bool  SetRegisterClassWindow(HINSTANCE hInstance);
    bool  SetWindow();
    bool  Run();
};

