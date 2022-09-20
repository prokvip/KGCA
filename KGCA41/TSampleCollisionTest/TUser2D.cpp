#include "TUser2D.h"
#include "TInput.h"
bool TUser2D::Frame()
{
    TVector2D vPos = m_vPos;
    
    m_vDir = { 0,0 };
    if (I_Input.GetKey('W'))
    {
        m_vDir.y = -1.0f;
        vPos.y += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('S'))
    {
        m_vDir.y = 1.0f;
        vPos.y += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('A'))
    {
        m_vDir.x = -1.0f;
        vPos.x += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('D'))
    {
        m_vDir.x = 1.0f;
        vPos.x += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    m_vPos = vPos;   
    return true;
}