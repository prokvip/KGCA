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
    g_Input.Init();
    m_Write.Init();

    IDXGISurface1* m_pBackBuffer;
    m_pSwapChain->GetBuffer(0, 
        __uuidof(IDXGISurface),
        (void**)&m_pBackBuffer);
    m_Write.CreateDeviceResources(m_pBackBuffer);
    if (m_pBackBuffer)m_pBackBuffer->Release();

    Init();
    return true;
}
bool	TCore::GameFrame() 
{
    // TODO : Frame Timer
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    
    if (g_Input.GetKey('1') == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }
    Frame();
    return true;
}
bool	TCore::GameRender() 
{
    PreRender();   
        // TODO : Render Timer
        m_Timer.Render();
        g_Input.Render();
        m_Write.Render();
        if (m_bDebugText)
        {
            RECT  rt = { 0, 0, 800, 600 };
            m_Write.DrawText(rt, m_Timer.m_szTimerString,
                D2D1::ColorF(1, 1, 1, 1));
        }
        Render();
    PostRender();    
    return true;
}
bool	TCore::GameRelease() 
{
    Release();
    // TODO : Render Timer
    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
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