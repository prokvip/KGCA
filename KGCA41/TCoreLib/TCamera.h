#pragma once
#include "TStd.h"

class xPlane
{
public:
    float   fa, fb, fc, fd;
    void	Normalize()
    {
        float fMag = sqrt(fa * fa + fb * fb + fc * fc);
        fa = fa / fMag;
        fb = fb / fMag;
        fc = fc / fMag;
        fd = fd / fMag;
    }
    void   CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
    {
        TVector3 vE0 = v1 - v0;
        TVector3 vE1 = v2 - v0;
        TVector3 vNormal;
        D3DXVec3Cross(&vNormal, &vE0, &vE1);
        D3DXVec3Normalize(&vNormal, &vNormal);
        fa = vNormal.x;
        fb = vNormal.y;
        fc = vNormal.z;
        fd = -(fa * v0.x + fb * v0.y + fc * v0.z);
    }
    void   CreatePlane(TVector3 v0, TVector3 vNormal)
    {
        D3DXVec3Normalize(&vNormal, &vNormal);
        fa = vNormal.x;
        fb = vNormal.y;
        fc = vNormal.z;
        fd = -(fa * v0.x + fb * v0.y + fc * v0.z);
    }
};
class TFrustum
{
public:
    TVector3  m_vFrustum[8];
    xPlane    m_xPlane[6];
    void      Create(TMatrix matView, TMatrix matProj)
    {
        TMatrix mat = matView * matProj;
       // D3DXMatrixMultiply(&mat, &matView, &matProj);
        D3DXMatrixInverse(&mat,NULL, &mat);

        //ExtractPlanesD3D(mat);
        // 5   6
        // 4   7
        // 
        // 1   2
        // 0   3
        m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
        m_vFrustum[1] = TVector3(-1.0f, 1.0f, 0.0f);
        m_vFrustum[2] = TVector3(1.0f, 1.0f, 0.0f);
        m_vFrustum[3] = TVector3(1.0f, -1.0f, 0.0f);

        m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
        m_vFrustum[5] = TVector3(-1.0f, 1.0f, 1.0f);
        m_vFrustum[6] = TVector3(1.0f, 1.0f, 1.0f);
        m_vFrustum[7] = TVector3(1.0f, -1.0f, 1.0f);

        for (int i = 0; i < 8; i++)
        {
            D3DXVec3TransformCoord(&m_vFrustum[i],
                &m_vFrustum[i], &mat);
        }
        
        m_xPlane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	// 좌 평면(left)
        m_xPlane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// 우 평면(right)
        m_xPlane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// 상 평면(top)
        m_xPlane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	// 하 평면(bottom)
        m_xPlane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	// 근 평면(near)
        m_xPlane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// 원 평면(far)
    }

    void ExtractPlanesD3D(const TMatrix& comboMatrix)
    {
        // Left clipping plane
        m_xPlane[0].fa = comboMatrix._14 + comboMatrix._11;
        m_xPlane[0].fb = comboMatrix._24 + comboMatrix._21;
        m_xPlane[0].fc = comboMatrix._34 + comboMatrix._31;
        m_xPlane[0].fd = comboMatrix._44 + comboMatrix._41;
        // Right clipping plane
        m_xPlane[1].fa = comboMatrix._14 - comboMatrix._11;
        m_xPlane[1].fb = comboMatrix._24 - comboMatrix._21;
        m_xPlane[1].fc = comboMatrix._34 - comboMatrix._31;
        m_xPlane[1].fd = comboMatrix._44 - comboMatrix._41;
        // Top clipping plane
        m_xPlane[2].fa = comboMatrix._14 - comboMatrix._12;
        m_xPlane[2].fb = comboMatrix._24 - comboMatrix._22;
        m_xPlane[2].fc = comboMatrix._34 - comboMatrix._32;
        m_xPlane[2].fd = comboMatrix._44 - comboMatrix._42;
        // Bottom clipping plane
        m_xPlane[3].fa = comboMatrix._14 + comboMatrix._12;
        m_xPlane[3].fb = comboMatrix._24 + comboMatrix._22;
        m_xPlane[3].fc = comboMatrix._34 + comboMatrix._32;
        m_xPlane[3].fd = comboMatrix._44 + comboMatrix._42;
        // Near clipping plane
        m_xPlane[4].fa = comboMatrix._13;
        m_xPlane[4].fb = comboMatrix._23;
        m_xPlane[4].fc = comboMatrix._33;
        m_xPlane[4].fd = comboMatrix._43;
        // Far clipping plane
        m_xPlane[5].fa = comboMatrix._14 - comboMatrix._13;
        m_xPlane[5].fb = comboMatrix._24 - comboMatrix._23;
        m_xPlane[5].fc = comboMatrix._34 - comboMatrix._33;
        m_xPlane[5].fd = comboMatrix._44 - comboMatrix._43;


        m_xPlane[0].Normalize();
        m_xPlane[1].Normalize();
        m_xPlane[2].Normalize();
        m_xPlane[3].Normalize();
        m_xPlane[4].Normalize();
        m_xPlane[5].Normalize();
    }
};
class TCamera
{
public:
    TVector3   m_vLook;
    TVector3   m_vUp;
    TVector3   m_vSide;
    float      m_fSpeed = 10.0f;

    TFrustum   m_Frustum;
public:
    TBASIS_EX::TMatrix                 m_matView;
    TBASIS_EX::TMatrix                 m_matOrthoProjection;
    TBASIS_EX::TMatrix                 m_matProj;
    TVector3                m_vCameraPos;
    TVector3                m_vTargetPos;
    DWORD                   m_dwClientWidth;
    DWORD                   m_dwClientHeight;
public:
    TBASIS_EX::TMatrix   CreateLookAt(TVector3 p, TVector3 t, TVector3 n = { 0,1,0 });
    TBASIS_EX::TMatrix   CreatePerspectiveFov(float fFov, float fAspect, float fNear, float fFar);
    bool      Frame2D();
    void      UpdateVector();
public:
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
public:
    bool  Create(TVector3 vPos, TVector2 size);
};

