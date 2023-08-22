#pragma once
#include "TStd.h"
class TCamera
{
public:
    TMatrix                 m_matView;
    TMatrix                 m_matOrthoProjection;
    TVector3                m_vCameraPos;
    DWORD                   m_dwWindowWidth;
    DWORD                   m_dwWindowHeight;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
public:
    bool  Create(TVector3 vPos, TVector2 size);
};

