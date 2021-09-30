#include "Sample.h"
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sample g_Sample;
    g_Sample.InitWindows(
        hInstance,
        nCmdShow,
        L"CreateWindows");
    g_Sample.Run();
    //MessageBox(NULL, L"Exit", L"Click", MB_OK);
}