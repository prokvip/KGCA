#include "TGamePlayer.h"

void TGamePlayer::Frame(float fDeltaTime, float fGameTime)
{    
    m_fSpeed = 10.0f;
    m_fMass = 100.0f;
    TVector2D vPos(m_rt.x1, m_rt.y1);
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
 
    TVector2D vKeyForce = m_vDirection*m_fSpeed* fDeltaTime;
    AddForces(vKeyForce);

    m_vAcceleration = m_vForces / m_fMass;
    TVector2D linearAcc = m_vAcceleration *fDeltaTime;
    m_vVelocity += linearAcc;
    //������ ������ ������
    //������� = ���ۺ��� + ���⺤��* speed * t(�Ÿ� ������ �ð�) 
    vPos = vPos + m_vVelocity;
    
    if (vPos.x > 100.0f)
    {
        vPos.x = 100.0f;
        m_vForces = TVector2D(0, 0);
        m_vVelocity = TVector2D(0, 0);
        m_vDirection *= -1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vForces = TVector2D(0, 0);
        m_vVelocity = TVector2D(0, 0);
        m_vDirection.x = 1.0f;
    }
    if (vPos.y > 100.0f)
    {
        vPos.y = 100.0f;
        m_vForces = TVector2D(0, 0);
        m_vDirection.y = -1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vForces = TVector2D(0, 0);
        m_vVelocity = TVector2D(0, 0);
        m_vDirection.y = 1.0f;
    }

    m_vDirection.Normalized();
    SetPosition(vPos.x, vPos.y, m_rt.w, m_rt.h);
}
