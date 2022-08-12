#include "TObject.h"
void        TObject::AddForces(TVector2D f)
{
    m_vForces += f;
}
void   TObject::SetCircle(float x, float y, float w, float h)
{
    m_Circle.cx = x;
    m_Circle.cy = y;
    float x1 = w / 2.0f;
    float y1 = h / 2.0f;
    m_Circle.fRadius = sqrt(x1 * x1 + y1 * y1);
}
void   TObject::SetPosition(float x, float y, float w, float h)
{
    m_rt.Set(x, y, w, h);
    SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
}

void   TObject::Frame(float fDeltaTime, float fGameTime) {};

TObject::TObject()
{
    m_rt.Set(20 + (rand() % 80),
        20 + (rand() % 80),
        5.0f + (rand() % 5),
        5.0f + (rand() % 5));
    SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
    m_vDirection.x = cos(rand());
    m_vDirection.y = sin(rand());
    m_vDirection.Normalized();
    m_vFriction = TVector2D(10,10);
    m_fMass = 100.0f;
    m_fSpeed = 10.0f;    
}