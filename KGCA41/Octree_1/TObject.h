#pragma once
#include "TCollision.h"

class TObject
{
public:
    TVector    m_vDirection;
    TBox       m_Box;
    TSphere    m_Sphere;
public:
    TVector   m_vAcceleration;
    TVector   m_vVelocity;
    TVector   m_vForces;
    float       m_fFriction;
    float       m_fMass;    
    float       m_fSpeed;
public:
    void   SetSphere(TVector vCenter, float radius);
    void   SetSphere(TBox box);
    void   SetPosition(TVector p, TVector s);
public:
    virtual void   Frame(float fDeltaTime, float fGameTime);
    virtual void   AddForces(TVector f);
public:
    TObject();
};
