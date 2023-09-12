#pragma once
#include "TDxObject.h"
class TObject : public TDxObject
{     
public:
    bool            m_bDead=false;
    int             m_iCollisionID = -1;
    int             m_iSelectID = -1;
    virtual void    MouseOverlap(TVector2 pt, DWORD state)
    {
        int k = 0;
    }
public:
    TVector3        m_vPos;
    TVector3        m_vScale;
    TVector3        m_vRotation;
    TMatrix         m_matWorld;
    TMatrix         m_matView;
    TMatrix         m_matProj;
    TRect	        m_tRT;
    W_STR           m_szName;
    void            SetName(W_STR name) { m_szName = name; }
    virtual void    SetRect(TVector2& c, float w, float h);
public:
    void          SetPos(TVector3 p);
    void          SetScale(TVector3 s);
public:
    virtual bool  Create(std::wstring shaderFilename,
                         std::wstring texFilename);
    virtual void  UpdateMatrix();
    virtual bool  Init() override;
    virtual bool  Frame()override;
    virtual bool  Render()override;
    virtual bool  Release()override;
    virtual void  SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
    virtual void  Move(float fSecond) {};
public:
    TObject();
    virtual ~TObject() {}
};

