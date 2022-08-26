#include "Sample.h"
bool		Sample::Init()
{   
    TDevice::Init();
    return true;
}
bool		Sample::Frame()
{
    TDevice::Frame();
    return true;
}
bool		Sample::Render()
{
    TDevice::Render();
    return true;
}
bool		Sample::Release()
{
    TDevice::Release();
    return true;
}


GAME_RUN(P2_CreateDevice_0, 1024, 768)
