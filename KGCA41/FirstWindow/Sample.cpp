#include "TWindow.h"

int APIENTRY wWinMain( HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPWSTR    lpCmdLine,  int       nCmdShow)
{
    TWindow win;
    win.SetRegisterClassWindow(hInstance);
    win.SetWindow();
    win.Init();
        win.Run();
    win.Release();
    return 0;
}