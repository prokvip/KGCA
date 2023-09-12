#pragma once
#include "TStd.h"
class TCamera
{
public:
    TMatrix                 m_matView;
    TMatrix                 m_matOrthoProjection;
    TMatrix                 m_matProj;
    TVector3                m_vCameraPos;
    DWORD                   m_dwWindowWidth;
    DWORD                   m_dwWindowHeight;
public:
    TMatrix   CreateLookAt(TVector3 p, TVector3 t, TVector3 n = { 0,1,0 });
    TMatrix   CreatePerspectiveFov(float fFov, float fAspect, float fNear, float fFar);
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
public:
    bool  Create(TVector3 vPos, TVector2 size);
};

