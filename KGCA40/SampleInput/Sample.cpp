#include "TCore.h"
#include "Sample.h"

bool	Sample::Init()
{    
    return true;
}
bool	Sample::Frame()
{
    if (g_Input.GetKey(VK_LBUTTON)== KEY_PUSH)
    {
        int kkkk = 0;
    }
    
    return true;
}
bool	Sample::Render() 
{
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