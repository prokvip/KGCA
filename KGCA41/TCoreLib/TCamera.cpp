#include "TCamera.h"
TBASIS_EX::TMatrix  TCamera::CreateLookAt(TVector3 vPosition, TVector3 vTarget, TVector3 vUp)
{
    m_vTargetPos = vTarget;
    m_vCameraPos = vPosition;

    TBASIS_EX::TVector3 vDirection = vTarget - vPosition;    
    D3DXVec3Normalize(&vDirection, &vDirection);
    float fDot = D3DXVec3Dot(&vUp, &vDirection);
    TVector3 vC = vDirection * fDot;
    TVector3 vUpVector = vUp - vC;
    D3DXVec3Normalize(&vUpVector, &vUpVector);
    TVector3 vRightVector;
    D3DXVec3Cross(&vRightVector,  &vUpVector, &vDirection);

    m_matView._11 = vRightVector.x;	m_matView._12 = vUpVector.x;		m_matView._13 = vDirection.x;
    m_matView._21 = vRightVector.y;	m_matView._22 = vUpVector.y;		m_matView._23 = vDirection.y;
    m_matView._31 = vRightVector.z;	m_matView._32 = vUpVector.z;		m_matView._33 = vDirection.z;

    m_matView._41 = -(vPosition.x * m_matView._11 + vPosition.y * m_matView._21 + vPosition.z * m_matView._31);
    m_matView._42 = -(vPosition.x * m_matView._12 + vPosition.y * m_matView._22 + vPosition.z * m_matView._32);
    m_matView._43 = -(vPosition.x * m_matView._13 + vPosition.y * m_matView._23 + vPosition.z * m_matView._33);
    
    UpdateVector();
    return m_matView;
}
void   TCamera::UpdateVector()
{
    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;
}
TBASIS_EX::TMatrix   TCamera::CreatePerspectiveFov(float fovy, float Aspect, float fNearPlane, float fFarPlane)
{
    float    h, w, Q;
    h = 1 / tan(fovy * 0.5f);
    w = h / Aspect;
    Q = fFarPlane / (fFarPlane - fNearPlane);
    m_matProj._44 = 0.0f;
    m_matProj._11 = w;
    m_matProj._22 = h;
    m_matProj._33 = Q;
    m_matProj._43 = -Q * fNearPlane;
    m_matProj._34 = 1;
    return m_matProj;
}

bool  TCamera::Create(TVector3 vPos, TVector2 size )
{
    m_vCameraPos = vPos;
    m_dwClientWidth = size.x;
    m_dwClientHeight = size.y;
    m_matView._41 = -m_vCameraPos.x;
    m_matView._42 = -m_vCameraPos.y;
    m_matView._43 = -m_vCameraPos.z;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwClientWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwClientHeight);
    return true;
}
bool  TCamera::Frame2D()
{
    float fHalfWidth = m_dwClientWidth / 2.0f;
    float fHalfHeight = m_dwClientHeight / 2.0f;
    if (m_vCameraPos.x < -(g_fMapHalfSizeX)+fHalfWidth)
    {
        m_vCameraPos.x = -(g_fMapHalfSizeX)+fHalfWidth;
    }
    if (m_vCameraPos.y < -(g_fMapHalfSizeY)+fHalfHeight)
    {
        m_vCameraPos.y = -(g_fMapHalfSizeY)+fHalfHeight;
    }
    if (m_vCameraPos.x > (g_fMapHalfSizeX) -fHalfWidth)
    {
        m_vCameraPos.x = (g_fMapHalfSizeX)-fHalfWidth;
    }
    if (m_vCameraPos.y > (g_fMapHalfSizeY)-fHalfHeight)
    {
        m_vCameraPos.y = (g_fMapHalfSizeY)-fHalfHeight;
    }

    m_matView._41 = -m_vCameraPos.x;
    m_matView._42 = -m_vCameraPos.y;
    m_matView._43 = -m_vCameraPos.z;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwClientWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwClientHeight);
    // ¿ùµåÁÂÇ¥ ¹üÀ§(-10 ~ +10)  camera (0,0)
    // -10 ~ +10 camera (-5,0)°¡ ¿øÁ¡ÀÌ µÈ´Ù.
    // ºä ÁÂÇ¥ -> -5 ~ 15
    // Åõ¿µÁÂÇ¥ -> 9 ~ 10 ~ 11
    // Åõ¿µÁÂÇ¥ -> -1 ~ 0 ~ +1
    return true;
}
bool  TCamera::Frame()
{    
    return true;
}
bool  TCamera::Init()
{
    return true;
}
bool  TCamera::Render()
{
    return true;
}
bool  TCamera::Release()
{
    return true;
}