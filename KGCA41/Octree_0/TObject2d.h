#pragma once
#include "TCollision.h"

class TObject2D
{
public:
    TVector2D   m_vDirection;
    TRect       m_rt;
    TCircle     m_Circle;
public:
    TVector2D   m_vAcceleration;
    TVector2D   m_vVelocity;
    TVector2D   m_vForces;
    float       m_fFriction;
    float       m_fMass;    
    float       m_fSpeed;
public:
    void   SetCircle(float x, float y, float w, float h);
    void   SetPosition(float x, float y, float w, float h);
public:
    virtual void   Frame(float fDeltaTime, float fGameTime);
    virtual void   AddForces(TVector2D f);
public:
    TObject2D();
};
