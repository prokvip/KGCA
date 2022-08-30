#pragma once
#include "TBaseObject.h"

class TObject2D : public TBaseObject
{
public:
    void   SetCircle(float x, float y, float w, float h);
    void   SetPosition(float x, float y, float w, float h);
    void   SetCircle(TVector2D p, TVector2D s);
    void   SetPosition(TVector2D p, TVector2D s);
public:
    virtual bool   Init() override;
    virtual bool   Frame(float fDeltaTime, float fGameTime)override;
    virtual bool   Render() override;
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
    virtual bool   Init() override;
    virtual bool   Frame(float fDeltaTime, float fGameTime)override;
    virtual bool   Render() override;
public:
    virtual void   AddForces(TVector f);
public:
    TObject();
    TObject(std::string name);
};