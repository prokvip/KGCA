#include "Sample.h"
#include "TVector.h"
TBASIS_RUN(SampleLibrary)
Sample::Sample()
{
    m_pSpeed = 3.0f;
    m_vCameraPos = { 3,3, -5.0f };
    m_vCameraTarget = { 0,0,0.0f };
}
bool Sample::Init()
{
    m_BoxObj.Init();
    return true;
}
bool Sample::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos.z += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_pSpeed * g_fSecPerFrame;
    }
    // -1 ~ +1 -> 0 ~ 1 => -1*0.5f+0.5f;
    // 0 ~ +1 -> -1 ~ 1 => 0.5f*2.0f-1.0f;
    // D3D11_USAGE_DEFAULT
    //m_cbData.matWorld._11 = cosf(g_fGameTimer)*0.5f+0.5f;
    m_cbData.matWorld = TMatrix::RotationZ(g_fGameTimer);

    TVector3 vUp = { 0,1,0.0f };
    m_cbData.matView = TMatrix::ViewLookAt(
        m_vCameraPos, m_vCameraTarget, vUp);
    m_cbData.matProj = TMatrix::PerspectiveFovLH(1.0f, 
        100.0f, TBASIS_PI * 0.5f, 
        (float)g_rtClient.right / (float)g_rtClient.bottom);

    return true;
}

bool Sample::Render()
{
    m_BoxObj.SetMatrix(
        &m_cbData.matWorld, 
        &m_cbData.matView, 
        &m_cbData.matProj);
    m_BoxObj.Render(m_pImmediateContext);
    return false;
}

bool Sample::Release()
{
    m_BoxObj.Release();
    return false;
}
