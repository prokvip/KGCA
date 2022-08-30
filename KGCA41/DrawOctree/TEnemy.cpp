#include "TEnemy.h"
TEnemy2D::TEnemy2D()
{
    m_fFriction = 1.0f;
}
TEnemy2D::TEnemy2D(std::string name) : TObject2D(name)
{
    m_fFriction = 1.0f;
}
bool   TEnemy2D::Render()
{
    TBaseObject::Render();
    std::cout << m_csName << ","
        << m_rt.x1 << ","
        << m_rt.y1 << std::endl;
    return true;
};
TEnemy::TEnemy()
{
    m_fFriction = 1.0f;
}
TEnemy::TEnemy(std::string name) : TObject(name)
{
    m_fFriction = 1.0f;
}
bool   TEnemy::Render() {
    std::cout << m_csName << ","
        << m_Box.vMin.x << ","
        << m_Box.vMin.y << ","
        << m_Box.vMin.z << std::endl;
    return true;
};

// 뉴턴의 운동법칙
// 1)관성의 법칙:상태를 유지 할려는 특성
// 2)가속도의 법칙: 힘(Force)에 비례하여 힘의 방향으로 움직인다.
//   가속도 : 시간에 따라서 속도의 순간적인 변화량
// 3)작용, 반작용 법칙
//   힘(작용)을 받은 물체는 반대방향으로 힘(반작용)이 가해진다. 
bool TEnemy::Frame(float fDeltaTime, float fGameTime)
{
    TVector vPos = m_Box.vMin;
    TVector vSize = m_Box.vSize;
    //벡터의 직선의 방정식 &  시간의 동기화
    m_vVelocity = m_vDirection* m_fSpeed* fDeltaTime;    
    vPos = vPos + m_vVelocity;
    m_fFriction = (fDeltaTime * m_fSpeed * 0.1f);
    m_fSpeed -= m_fFriction;
    if (0 >= m_fSpeed)
    {
        m_fSpeed = rand() % 10;
    }

    if (vPos.x > 100.0f)
    {
        vPos.x = 100.0f;
        m_vDirection.x *= -1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vDirection.x *= -1.0f;
    }
    if (vPos.y > 100.0f)
    {
        vPos.y = 100.0f;
        m_vDirection.y *= -1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vDirection.y *= -1.0f;
    }
    if (vPos.z > 100.0f)
    {
        vPos.z = 100.0f;
        m_vDirection.z *= -1.0f;
    }
    if (vPos.z < 0.0f)
    {
        vPos.z = 0.0f;
        m_vDirection.z *= -1.0f;
    }
    SetPosition(vPos, vSize);
    return true;
}
bool TEnemy2D::Frame(float fDeltaTime, float fGameTime)
{
    TVector2D vPos(m_rt.x1, m_rt.y1);
    TVector2D vSize(m_rt.w, m_rt.h);
    //벡터의 직선의 방정식 &  시간의 동기화
    m_vVelocity2D = m_vDirection2D * m_fSpeed * fDeltaTime;
    vPos = vPos + m_vVelocity2D;    
    m_fSpeed -= m_fFriction * fDeltaTime;
    if (0 >= m_fSpeed)
    {
        m_fSpeed = 10.0f  +rand() % 10;
    }

    if (vPos.x > 100.0f)
    {
        vPos.x = 100.0f;
        m_vDirection2D.x *= -1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vDirection2D.x *= -1.0f;
    }
    if (vPos.y > 100.0f)
    {
        vPos.y = 100.0f;
        m_vDirection2D.y *= -1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vDirection2D.y *= -1.0f;
    }
    SetPosition(vPos, vSize);
    return true;
}

void   TEnemy2D::SetPosition(TVector2D p, TVector2D s)
{
    float x1 = (p.x / 100.0f) * 2 - 1.0f;
    float y1 = -1.0f*((p.y / 100.0f) * 2 - 1.0f);
    float w1 = (s.x / 100.0f) * 2;
    float h1 = (s.y / 100.0f) * 2;

    m_VertexList[0].p = { x1, y1, 0.0f };
    m_VertexList[1].p = { x1 + w1, y1, 0.0f };
    m_VertexList[2].p = { x1, y1 - h1, 0.0f };
    m_VertexList[3].p = m_VertexList[2].p;
    m_VertexList[4].p = m_VertexList[1].p;
    m_VertexList[5].p = { x1 + w1, y1 - h1, 0.0f };

    m_VertexList[0].c = { 0, 0, 1.0f, 1.0f };
    m_VertexList[1].c = { 0, 0, 1.0f, 1.0f };
    m_VertexList[2].c = { 0, 0, 1.0f, 1.0f };
    m_VertexList[3].c = { 0, 0, 1.0f, 1.0f };
    m_VertexList[4].c = { 0, 0, 1.0f, 1.0f };
    m_VertexList[5].c = { 0, 0, 1.0f, 1.0f };
    m_rt.Set(p.x, p.y, s.x, s.y);
    SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);

}