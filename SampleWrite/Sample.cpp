#include "TCore.h"
#include "Sample.h"

bool	Sample::Init()
{    
    return true;
}
bool	Sample::Frame()
{
    
    
    return true;
}
bool	Sample::Render() 
{
    RECT  rt = { 400, 300, 800, 600 };
    m_Write.DrawText(rt, L"Game",
            D2D1::ColorF(1, 0, 0, 1), 
        m_Write.m_pTextFormat50);
    return true;
}
bool	Sample::Release() 
{
    return true;
}

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