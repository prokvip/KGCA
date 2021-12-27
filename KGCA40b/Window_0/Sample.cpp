#include "TWindow.h"
int WINAPI wWinMain(   HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPWSTR    lpCmdLine,
                         int       nCmdShow)
{ 
   TWindow win;
   if (win.WinRegisterClass(hInstance)==FALSE) return 1;
   if (win.SetWindow(L"Win Sample") == FALSE) return 1;
   win.WinRun();
   return 1;
}
