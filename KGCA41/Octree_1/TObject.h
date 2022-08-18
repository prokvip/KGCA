#pragma once
#include "TCollision.h"
#include <string>
class TBaseObject
{
public:
    std::string  m_csName;
    float       m_fFriction;
    float       m_fMass;
    float       m_fSpeed;
public:
    TVector    m_vDirection;
    TBox       m_Box;
    TSphere    m_Sphere;
    TVector    m_vAcceleration;
    TVector    m_vVelocity;
    TVector    m_vForces;
public:
    TVector2D   m_vDirection2D;
    TRect       m_rt;
    TCircle     m_Circle;
    TVector2D   m_vAcceleration2D;
    TVector2D   m_vVelocity2D;
    TVector2D   m_vForces2D;    
public:
    virtual void   Init() {};
    virtual void   Frame(float fDeltaTime, float fGameTime) {};
};

class TObject2D : public TBaseObject
{
public:
    void   SetCircle(float x, float y, float w, float h);
    void   SetPosition(float x, float y, float w, float h);
    void   SetCircle(TVector2D p, TVector2D s);
    void   SetPosition(TVector2D p, TVector2D s);
public:
    virtual void   Init() override;
    virtual void   Frame(float fDeltaTime, float fGameTime)override;
    virtual void   AddForces(TVector2D f);
public:
    TObject2D();
    TObject2D(std::string name);
};
class TObject : public TBaseObject
{
public:
    void   SetSphere(TVector vCenter, float radius);
    void   SetSphere(TBox box);
    void   SetPosition(TVector p, TVector s);
public:
    virtual void   Init() override;
    virtual void   Frame(float fDeltaTime, float fGameTime)override;
public:
    virtual void   AddForces(TVector f);
public:
    TObject();
    TObject(std::string name);
};