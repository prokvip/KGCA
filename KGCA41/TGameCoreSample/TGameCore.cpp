#include "TGameCore.h"

bool		TGameCore::TCoreInit()
{
    HRESULT hr;
    if (TDevice::Init() == false)
    {
        return false;
    }
    I_Timer.Init();
    I_Input.Init();
    return Init();
}
bool		TGameCore::TCoreFrame()
{
    I_Timer.Frame();
    I_Input.Frame();
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
        I_Timer.Render();
        I_Input.Render();
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
    I_Timer.Release();
    I_Input.Release();
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