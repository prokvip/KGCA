#include "TObject.h"
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