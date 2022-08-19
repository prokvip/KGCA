#include "TEnemy.h"
TEnemy2D::TEnemy2D()
{

}
TEnemy2D::TEnemy2D(std::string name) : TObject2D(name)
{

}
void   TEnemy2D::Render() 
{
    std::cout << m_csName << ","
        << m_rt.x1 << ","
        << m_rt.y1 << std::endl;
};
TEnemy::TEnemy()
{

}
TEnemy::TEnemy(std::string name) : TObject(name)
{

}
void   TEnemy::Render() {
    std::cout << m_csName << ","
        << m_Box.vMin.x << ","
        << m_Box.vMin.y << ","
        << m_Box.vMin.z << std::endl;
};

// ������ ���Ģ
// 1)������ ��Ģ:���¸� ���� �ҷ��� Ư��
// 2)���ӵ��� ��Ģ: ��(Force)�� ����Ͽ� ���� �������� �����δ�.
//   ���ӵ� : �ð��� ���� �ӵ��� �������� ��ȭ��
// 3)�ۿ�, ���ۿ� ��Ģ
//   ��(�ۿ�)�� ���� ��ü�� �ݴ�������� ��(���ۿ�)�� ��������. 
void TEnemy::Frame(float fDeltaTime, float fGameTime)
{
    TVector vPos = m_Box.vMin;
    TVector vSize = m_Box.vSize;
    //������ ������ ������ &  �ð��� ����ȭ
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
}
void TEnemy2D::Frame(float fDeltaTime, float fGameTime)
{
    TVector2D vPos(m_rt.x1, m_rt.y1);
    TVector2D vSize(m_rt.w, m_rt.h);
    //������ ������ ������ &  �ð��� ����ȭ
    m_vVelocity2D = m_vDirection2D * m_fSpeed * fDeltaTime;
    vPos = vPos + m_vVelocity2D;
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
    SetPosition(vPos, vSize);
}
