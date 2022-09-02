#include "TGameCore.h"
bool		TGameCore::TCoreInit()
{
    HRESULT hr;
    if (TDevice::Init() == false)
    {
        return false;
    }
    m_Timer.Init();
    m_Input.Init();
    return Init();
}
bool		TGameCore::TCoreFrame()
{
    m_Timer.Frame();
    m_Input.Frame();
    return Frame();
}
bool		TGameCore::TCorePreRender()
{
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, NULL);
    float color[4] = { 1.0f,1.0f,1.0f,1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);
    return true;
}
bool		TGameCore::TCoreRender()
{
    TCorePreRender();
        Render();
        m_Timer.Render();
        m_Input.Render();
    TCorePostRender();
    return true;
}
bool		TGameCore::TCorePostRender()
{
    m_pSwapChain->Present(0, 0);
    return true;
}
bool		TGameCore::TCoreRelease()
{
    Release();
    m_Timer.Release();
    m_Input.Release();
    TDevice::Release();
    return true;
}
bool        TGameCore::Run()
{
    if (TCoreInit() == false)
    {
        TCoreRelease();
        return false;
    }
    MSG msg = { 0, };
    m_bGameRun = true;   
    while (m_bGameRun)
    {
        if (TWindow::Run() == false)
        {
            m_bGameRun = false;
        }
        else
        {
            if (!TCoreFrame() || !TCoreRender())
            {
                m_bGameRun = false;
            }
        }     
    }
    TCoreRelease();
    return true;
}