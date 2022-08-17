#pragma once
#include "TCollision.h"
#include <string>
class TBaseObject
{
public:
    std::string  m_csName;
};

class TObject2D : public TBaseObject
{
public:
    TVector2D   m_vDirection2D;
    TRect       m_rt;
    TCircle     m_Circle;
public:
    TVector2D   m_vAcceleration2D;
    TVector2D   m_vVelocity2D;
    TVector2D   m_vForces2D;
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
class TObject : public TObject2D
{
public:
    TVector    m_vDirection;
    TBox       m_Box;
    TSphere    m_Sphere;
public:
    TVector   m_vAcceleration;
    TVector   m_vVelocity;
    TVector   m_vForces;
public:
    void   SetSphere(TVector vCenter, float radius);
    void   SetSphere(TBox box);
    void   SetPosition(TVector p, TVector s);
public:
    virtual void   Init();
    virtual void   Frame(float fDeltaTime, float fGameTime);
    virtual void   AddForces(TVector f);
public:
    TObject();
    TObject(std::string name);
};