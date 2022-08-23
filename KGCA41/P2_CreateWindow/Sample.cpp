#include "Sample.h"
bool		Sample::Init()
{
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

GAME_RUN(P2_CreateWidnow, 1024, 768)

//GAME_START1
//    Sample demo; 
//    demo.SetWindow(hInstance); 
//    demo.Run(); 
//    return 1;
//GAME_END