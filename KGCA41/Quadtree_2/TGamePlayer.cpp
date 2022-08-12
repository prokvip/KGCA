#include "TGamePlayer.h"

void TGamePlayer::Frame()
{
    TVector2D vPos(m_rt.x1, m_rt.y1);
    // ������ ������ ������
   //  ������� = ���ۺ��� + ���⺤��* t(�Ÿ� ������ �ð�) 
    TVector2D vTmp = m_vDirection * 1.0f;
    vPos = vPos + vTmp;
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
    SetPosition(vPos.x, vPos.y, 30, 30);
}
