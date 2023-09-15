#pragma once
#include "TStd.h"
class TCamera
{
public:
    TVector3   m_vLook;
    TVector3   m_vUp;
    TVector3   m_vSide;
    float      m_fSpeed = 10.0f;
public:
    TBASIS_EX::TMatrix                 m_matView;
    TBASIS_EX::TMatrix                 m_matOrthoProjection;
    TBASIS_EX::TMatrix                 m_matProj;
    TVector3                m_vCameraPos;
    TVector3                m_vTargetPos;
    DWORD                   m_dwWindowWidth;
    DWORD                   m_dwWindowHeight;
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

