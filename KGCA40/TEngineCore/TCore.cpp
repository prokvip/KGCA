#include "TCore.h"
LRESULT TCore::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
void TCore::FrameCamera()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    float fYaw = g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    float fPitch = g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_Camera.Update(TVector4(fPitch, fYaw, 0.0f, 0.0f));
    m_Camera.Frame();
}

bool	TCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	TCore::GameInit()
{
    TDevice::SetDevice();
    TDxState::Init();

    m_Timer.Init();
    g_Input.Init();
    m_Write.Init();

    m_Camera.Init();
    m_Camera.CreateViewMatrix(TVector3(0, 0, -100),TVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f,1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);

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
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    FrameCamera();

    if (g_Input.GetKey(VK_F2) >= KEY_PUSH)
    {
       ApplyRS(m_pImmediateContext, TDxState::g_pRSWireFrame);
    }
    else
    {
        ApplyRS(m_pImmediateContext, TDxState::g_pRSSolid);
    }
    if (g_Input.GetKey('1') == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }
    Frame();

    g_Input.m_ptBeforePos = g_Input.m_ptPos;
    return true;
}
bool	TCore::GameRender() 
{
    PreRender();   
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
    TDxState::Release();
    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
    m_Camera.Release();
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
bool	TCore::PreRender() 
{
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(
        m_DefaultRT.m_pRenderTargetView, ClearColor);
    m_pImmediateContext->ClearDepthStencilView(
        m_DefaultDS.m_pDepthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->OMSetRenderTargets(1,
        &m_DefaultRT.m_pRenderTargetView, m_DefaultDS.m_pDepthStencilView);

    ApplyDSS(m_pImmediateContext, TDxState::g_pLessEqualDSS);
    ApplySS(m_pImmediateContext, TDxState::g_pWrapSS,0);
    ApplyRS(m_pImmediateContext, TDxState::g_pRSSolid);
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