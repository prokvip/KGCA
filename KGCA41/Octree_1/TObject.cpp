#include "TObject.h"

void   TObject2D::AddForces(TVector2D f)
{
    m_vForces2D += f;
}
void   TObject2D::SetCircle(TVector2D c, TVector2D s)
{
    m_Circle.cx = c.x;
    m_Circle.cy = c.y;
    float x1 = s.x / 2.0f;
    float y1 = s.y / 2.0f;
    m_Circle.fRadius = sqrt(x1 * x1 + y1 * y1);
}
void   TObject2D::SetPosition(TVector2D p, TVector2D s)
{
    m_rt.Set(p.x, p.y, s.x, s.y);
    SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
}
void   TObject2D::SetCircle(float x, float y, float w, float h)
{
    m_Circle.cx = x;
    m_Circle.cy = y;
    float x1 = w / 2.0f;
    float y1 = h / 2.0f;
    m_Circle.fRadius = sqrt(x1 * x1 + y1 * y1);
}
void   TObject2D::SetPosition(float x, float y, float w, float h)
{
    m_rt.Set(x, y, w, h);
    SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
}

void   TObject2D::Frame(float fDeltaTime, float fGameTime) {};
void   TObject2D::Init()
{
    m_rt.Set(20 + (rand() % 80),
        20 + (rand() % 80),
        5.0f + (rand() % 5),
        5.0f + (rand() % 5));
    SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
    m_vDirection2D.x = cos(rand());
    m_vDirection2D.y = sin(rand());
    m_vDirection2D.Normalized();
    m_fFriction = 0.0f;
    m_fMass = 100.0f;
    m_fSpeed = 10.0f;
}
TObject2D::TObject2D()
{
    Init();
}
TObject2D::TObject2D(std::string name)
{
    Init();
    m_csName = name;
}
////////////////////////////////////////////////////////
void        TObject::AddForces(TVector f)
{
    m_vForces += f;
}
void   TObject::SetSphere(TVector vCenter, float radius)
{
    m_Sphere.vCenter = vCenter;
    m_Sphere.fRadius = radius;
}
void   TObject::SetSphere(TBox box)
{
    m_Sphere.vCenter = box.vCenter;    
    // v0  -vC-  v1
    m_Sphere.fRadius = (box.vSize*0.5f).Length();    
}
void   TObject::SetPosition(TVector vPos, TVector vSize)
{
    m_Box.Set(vPos, vSize);
    SetSphere(m_Box);
}

void   TObject::Frame(float fDeltaTime, float fGameTime) {};

void   TObject::Init()
{
    TVector vPos, vSize;
    vPos.x = 20 + (rand() % 80);
    vPos.y = 20 + (rand() % 80);
    vPos.z = 20 + (rand() % 80);
    vSize.x = 5.0f + (rand() % 5);
    vSize.y = 5.0f + (rand() % 5);
    vSize.z = 5.0f + (rand() % 5);
    m_Box.Set(vPos, vSize);
    SetSphere(m_Box);
    m_vDirection.x = cos(rand());
    m_vDirection.y = sin(rand());
    m_vDirection.z = 1.0f - cos(rand());
    m_vDirection.Normalized();
    m_fFriction = 0.0f;
    m_fMass = 100.0f;
    m_fSpeed = 10.0f;
}
TObject::TObject()
{
    Init();
}
TObject::TObject(std::string name)
{
    Init();
    m_csName = name;
}