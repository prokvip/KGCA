#include "TCore.h"
bool	TCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	TCore::GameInit()
{
    TDevice::SetDevice();
    // TODO : create Timer
    m_Timer.Init();
    m_Input.Init();

    Init();
    return true;
}
bool	TCore::GameFrame() 
{
    // TODO : Frame Timer
    m_Timer.Frame();
    m_Input.Frame();
    Frame();
    return true;
}
bool	TCore::GameRender() 
{
    PreRender();   
        // TODO : Render Timer
        m_Timer.Render();
        m_Input.Render();
        Render();
    PostRender();    
    return true;
}
bool	TCore::GameRelease() 
{
    Release();
    // TODO : Render Timer
    m_Timer.Release();
    m_Input.Release();
    CleanupDevice();
    return true;
}

bool	TCore::Init()
{
    return true;
}
bool	TCore::Frame() {
    return true;
}
bool	TCore::PreRender() {
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    return true;
}
bool	TCore::Render() {
    return true;
}
bool	TCore::PostRender() {
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool	TCore::Release() {
    return true;
}