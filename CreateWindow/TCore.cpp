#include "TCore.h"
bool	TCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	TCore::GameInit()
{
    Init();
    return true;
}
bool	TCore::GameFrame() 
{
    Frame();
    return true;
}
bool	TCore::GameRender() 
{
    Render();
    return true;
}
bool	TCore::GameRelease() 
{
    Release();
    return true;
}

bool	TCore::Init()
{
    return true;
}
bool	TCore::Frame() {
    return true;
}
bool	TCore::Render() {
    return true;
}
bool	TCore::Release() {
    return true;
}