#include "Sample.h"
bool		Sample::Init()
{
    TWindow::Init();
    return true;
}
bool		Sample::Frame()
{
    return true;
}
bool		Sample::Render()
{
    return true;
}
bool		Sample::Release()
{
    return true;
}

int APIENTRY wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR    lpCmdLine,
    int       nCmdShow)
{
    Sample demo;
    demo.m_hInstance = hInstance;
    demo.Init();
    demo.Run();
    demo.Release();
    //MessageBox(NULL, L"test", L"title", MB_OK);
    return 1;
}