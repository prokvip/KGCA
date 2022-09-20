#include "TNpc2D.h"
bool TNpc2D::Frame()
{
    TVector2D vPos = m_vPos;
    ////������ ������ ������ &  �ð��� ����ȭ
    TVector2D m_vVelocity = m_vDir * m_fSpeed * g_fSecondPerFrame;
    vPos = vPos + m_vVelocity;

    if (vPos.x > 1000.0f)
    {
        vPos.x = 1000.0f;
        m_vDir.x *= -1.0f;
    }
    if (vPos.x < -1000.0f)
    {
        vPos.x = -1000.0f;
        m_vDir.x *= -1.0f;
    }
    if (vPos.y > 1000.0f)
    {
        vPos.y = 1000.0f;
        m_vDir.y *= -1.0f;
    }
    if (vPos.y < -1000.0f)
    {
        vPos.y = -1000.0f;
        m_vDir.y *= -1.0f;
    }

    SetPosition(vPos, m_vCameraPos);
    return true;
}
