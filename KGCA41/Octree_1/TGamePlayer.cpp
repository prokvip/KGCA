#include "TGamePlayer.h"

void TGamePlayer::Frame(float fDeltaTime, float fGameTime)
{    
    m_fSpeed = 30.0f;
    m_fMass = 10.0f;
    TVector vPos = m_Box.vMin;
    TVector vSize = m_Box.vSize;
    // ������ ������ ������
    // ������� = ���ۺ��� + ���⺤��* t(�Ÿ� ������ �ð�) 
    // ������ġ = ������ġ + ���⺤��* �ӷ�
    // �ӵ�(����) = ���⺤��*�ӷ�(m_fSpeed)
    // ������ġ = ������ġ + �ӵ�;
    // vPos = vPos + m_vVelocity* fDeltaTime(�ð�:0.1f);
    // time = ���� ������ �ð� - ���� ������ �ð�
    // 1�ʿ� 10,0,0 ���� ��������.
    // 0.5f   5,0,0
    // 0.1f   1,0,0

    //���ӵ��� ��Ģ   Force=Mass(����)*Acceleration(���ӵ�)
    // Force =  �ټ��� ������ ���̴�.
    // F=MA
    // A = F/M
    // V(�ӵ�) += A * t
    // ������ġ = ������ġ + V;
    // �ӵ� -= t ( max(0, velocity))
 
    TVector vKeyForce = m_vDirection*m_fSpeed* fDeltaTime;
    AddForces(vKeyForce);

    m_vAcceleration = m_vForces / m_fMass;
    TVector linearAcc = m_vAcceleration;
    m_vVelocity += linearAcc;
    //������ ������ ������
    //������� = ���ۺ��� + ���⺤��* speed * t(�Ÿ� ������ �ð�) 
    vPos = vPos + m_vVelocity;
    
    if ( /*fabs(m_vForces.Length()) <= T_Epsilon &&*/
         fabs(m_vVelocity.Length()) > T_Epsilon )
    {        
        m_fFriction -= fDeltaTime;
        if (0 >= m_fFriction)
        {               
            m_vVelocity = { 0,0,0 };
            m_fFriction = 1.0f;
        }
        m_vVelocity *= m_fFriction;
    }
    if (vPos.x > 100.0f)
    {
        vPos.x = 100.0f;
        m_vForces = TVector(0, 0, 0);
        m_vVelocity = TVector(0, 0, 0);
        m_vDirection *= -1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vForces = TVector(0, 0, 0);
        m_vVelocity = TVector(0, 0, 0);
        m_vDirection.x = 1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.y > 100.0f)
    {
        vPos.y = 100.0f;
        m_vForces = TVector(0, 0,0);
        m_vDirection.y = -1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vForces = TVector(0, 0,0);
        m_vVelocity = TVector(0, 0,0);
        m_vDirection.y = 1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.z > 100.0f)
    {
        vPos.z = 100.0f;
        m_vForces = TVector(0, 0, 0);
        m_fFriction = 1.0f;
        m_vDirection.z *= -1.0f;
    }
    if (vPos.z < 0.0f)
    {
        vPos.z = 0.0f;
        m_vForces = TVector(0, 0, 0);
        m_vVelocity = TVector(0, 0, 0);
        m_fFriction = 1.0f;
        m_vDirection.z *= -1.0f;
    }
    m_vDirection.Normalized();
    SetPosition(vPos, vSize);
}
void TGamePlayer2D::Frame(float fDeltaTime, float fGameTime)
{
    m_fSpeed = 30.0f;
    m_fMass = 10.0f;
    TVector2D vPos(m_rt.x1, m_rt.y1);
    TVector2D vSize(m_rt.w, m_rt.h);
    TVector2D vKeyForce = m_vDirection2D * m_fSpeed * fDeltaTime;
    AddForces(vKeyForce);

    m_vAcceleration2D = m_vForces2D / m_fMass;
    TVector2D linearAcc = m_vAcceleration2D;
    m_vVelocity2D += linearAcc;
    //������ ������ ������
    //������� = ���ۺ��� + ���⺤��* speed * t(�Ÿ� ������ �ð�) 
    vPos = vPos + m_vVelocity2D;

    if ( /*fabs(m_vForces2D.Length()) <= T_Epsilon &&*/
        fabs(m_vVelocity2D.Length()) > T_Epsilon)
    {
        m_fFriction -= fDeltaTime;
        if (0 >= m_fFriction)
        {
            m_vVelocity2D = { 0,0 };
            m_fFriction = 1.0f;
        }
        m_vVelocity2D *= m_fFriction;
    }
    if (vPos.x > 100.0f)
    {
        vPos.x = 100.0f;
        m_vForces2D = TVector2D(0, 0);
        m_vVelocity2D = TVector2D(0, 0);
        m_vDirection2D *= -1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vForces2D = TVector2D(0, 0);
        m_vVelocity2D = TVector2D(0, 0);
        m_vDirection2D.x = 1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.y > 100.0f)
    {
        vPos.y = 100.0f;
        m_vForces2D = TVector2D(0, 0);
        m_vDirection2D.y = -1.0f;
        m_fFriction = 1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vForces2D = TVector2D(0, 0);
        m_vVelocity2D = TVector2D(0, 0);
        m_vDirection2D.y = 1.0f;
        m_fFriction = 1.0f;
    }    
    m_vDirection2D.Normalized();
    SetPosition(vPos, vSize);
}
