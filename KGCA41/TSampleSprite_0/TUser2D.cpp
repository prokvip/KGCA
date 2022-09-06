#include "TUser2D.h"
#include "TInput.h"
bool TUser2D::Frame()
{
    TVector2D vPos = m_vPos;
    if (I_Input.GetKey('W'))
    {
        vPos.y += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('S'))
    {
        vPos.y += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('A'))
    {
        vPos.x += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('D'))
    {
        vPos.x += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }

    SetPosition(vPos);
    return true;
}