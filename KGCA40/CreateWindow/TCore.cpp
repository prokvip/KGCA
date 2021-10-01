#include "TCore.h"
bool	TCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	TCore::GameInit()
{
    HRESULT hr;
    if (FAILED(CreateGIFactory()))
    {
        return false;
    }
    if (FAILED(CreateDevice()))
    {
        return false;
    }
    if (FAILED(CreateSwapChain(m_hWnd,
        m_rtClient.right,
        m_rtClient.bottom)))
    {
        return false;
    }
    if (FAILED(SetRenderTargetView()))
    {
        return false;
    }
    if (FAILED(SetViewPort()))
    {
        return false;
    }


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
    PreRender();   
        Render();
    PostRender();    
    return true;
}
bool	TCore::GameRelease() 
{
    Release();

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