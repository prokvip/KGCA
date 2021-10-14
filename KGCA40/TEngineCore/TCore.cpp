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
bool    TCore::SetDeviceState()
{
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_BACK;
    g_pd3dDevice->CreateRasterizerState(&rd, &m_pRSWireFrame);
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    g_pd3dDevice->CreateRasterizerState(&rd, &m_pRSSolid);
    return true;
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
    SetDeviceState();
    // TODO : create Timer
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
    // TODO : Frame Timer
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    FrameCamera();

    if (g_Input.GetKey(VK_F2) >= KEY_PUSH)
    {
        m_pImmediateContext->RSSetState(m_pRSWireFrame);
    }
    else
    {
        m_pImmediateContext->RSSetState(m_pRSSolid);
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
    if(m_pRSSolid)m_pRSSolid->Release();
    if (m_pRSWireFrame)m_pRSWireFrame->Release();
    m_pRSSolid = nullptr;
    m_pRSWireFrame = nullptr;

    // TODO : Render Timer
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