#include "TCamera.h"
TMatrix  TCamera::CreateLookAt(TVector3 vPosition, TVector3 vTarget, TVector3 vUp)
{
    TVector3 vDirection = vTarget - vPosition;
    vDirection = vDirection.NormalVector();
    float fDot = vUp | vDirection;
    TVector3 vC = vDirection * fDot;
    TVector3 vUpVector = vUp - vC;
    vUpVector = vUpVector.NormalVector();
    TVector3 vRightVector = vUpVector ^ vDirection;

    m_matView._11 = vRightVector.x;	m_matView._12 = vUpVector.x;		m_matView._13 = vDirection.x;
    m_matView._21 = vRightVector.y;	m_matView._22 = vUpVector.y;		m_matView._23 = vDirection.y;
    m_matView._31 = vRightVector.z;	m_matView._32 = vUpVector.z;		m_matView._33 = vDirection.z;

    m_matView._41 = -(vPosition.x * m_matView._11 + vPosition.y * m_matView._21 + vPosition.z * m_matView._31);
    m_matView._42 = -(vPosition.x * m_matView._12 + vPosition.y * m_matView._22 + vPosition.z * m_matView._32);
    m_matView._43 = -(vPosition.x * m_matView._13 + vPosition.y * m_matView._23 + vPosition.z * m_matView._33);
    return m_matView;
}
TMatrix   TCamera::CreatePerspectiveFov(float fovy, float Aspect, float fNearPlane, float fFarPlane)
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
    m_dwWindowWidth = size.x;
    m_dwWindowHeight = size.y;
    m_matView._41 = -m_vCameraPos.x;
    m_matView._42 = -m_vCameraPos.y;
    m_matView._43 = -m_vCameraPos.z;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwWindowWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwWindowHeight);
    return true;
}
bool  TCamera::Frame()
{
    float fHalfWidth  = m_dwWindowWidth / 2.0f;
    float fHalfHeight = m_dwWindowHeight / 2.0f;
	if (m_vCameraPos.x < -(g_fMapHalfSizeX) + fHalfWidth)
	{
        m_vCameraPos.x = -(g_fMapHalfSizeX) + fHalfWidth;
	}
	if (m_vCameraPos.y < -(g_fMapHalfSizeY ) + fHalfHeight)
	{
        m_vCameraPos.y = -(g_fMapHalfSizeY ) + fHalfHeight;
	}
	if (m_vCameraPos.x > (g_fMapHalfSizeX ) - fHalfWidth)
	{
        m_vCameraPos.x = (g_fMapHalfSizeX ) - fHalfWidth;
	}
	if (m_vCameraPos.y > (g_fMapHalfSizeY ) - fHalfHeight)
	{
        m_vCameraPos.y = (g_fMapHalfSizeY ) - fHalfHeight;
	}

    m_matView._41 = -m_vCameraPos.x;
    m_matView._42 = -m_vCameraPos.y;
    m_matView._43 = -m_vCameraPos.z;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwWindowWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwWindowHeight);
    // ¿ùµåÁÂÇ¥ ¹üÀ§(-10 ~ +10)  camera (0,0)
    // -10 ~ +10 camera (-5,0)°¡ ¿øÁ¡ÀÌ µÈ´Ù.
    // ºä ÁÂÇ¥ -> -5 ~ 15
    // Åõ¿µÁÂÇ¥ -> 9 ~ 10 ~ 11
    // Åõ¿µÁÂÇ¥ -> -1 ~ 0 ~ +1
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