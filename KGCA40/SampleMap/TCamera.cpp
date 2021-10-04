#include "TCamera.h"
#include "TInput.h"
bool TCamera::Init()
{
    return true;
}
TMatrix     TCamera::CreateViewMatrix(
    TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vCameraTarget = vTarget;
    m_matView= TMatrix::ViewLookAt(
        m_vCameraPos, m_vCameraTarget, vUp);
    return m_matView;
}
TMatrix  	TCamera::CreateProjMatrix(
    float fNear, float fFar, float fFov, float fAspect)
{
    m_matProj = TMatrix::PerspectiveFovLH(fNear,
        fFar, fFov,
        fAspect);
    return m_matProj;
}
bool TCamera::Frame()
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
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraPos.y -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('R') >= KEY_HOLD)
    {
        m_vCameraPos.y += m_pSpeed * g_fSecPerFrame;
    }

    m_matView = CreateViewMatrix(m_vCameraPos, m_vCameraTarget);
    return true;
}
bool TCamera::Render()
{
    return true;
}
bool TCamera::Release()
{
    return true;
}
TCamera::TCamera()
{
    m_pSpeed = 3.0f;
    m_vCameraPos = { 8, 20, -20.0f };
    m_vCameraTarget = { 8, 0, -19.0f };
}

TCamera::~TCamera()
{
}