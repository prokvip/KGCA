#include "TPlayer.h"
#include "TInput.h"
bool TPlayer::Frame()
{
    if (TInput::GetInstance().m_dwKeyState['A'] > KEY_UP)
    {
        m_vPos.x -= 500.0f * g_fSecondPerFrame;
    }
    if (TInput::GetInstance().m_dwKeyState['D'] > KEY_UP)
    {
        m_vPos.x += 500.0f * g_fSecondPerFrame;
    }
    if (TInput::GetInstance().m_dwKeyState['W'] > KEY_UP)
    {
        m_vPos.y += 500.0f * g_fSecondPerFrame;
    }
    if (TInput::GetInstance().m_dwKeyState['S'] > KEY_UP)
    {
        m_vPos.y -= 500.0f * g_fSecondPerFrame;
    }

    float fSizeHalfWidth = m_vScale.x;
    float fSizeHalfHeight = m_vScale.y;
    if (m_vPos.x < -g_fMapSizeX + fSizeHalfWidth)
    {
        m_vPos.x = -g_fMapSizeX + fSizeHalfWidth;
    }
    if (m_vPos.y < -g_fMapSizeY + fSizeHalfHeight)
    {
        m_vPos.y = -g_fMapSizeY + fSizeHalfHeight;
    }
    if (m_vPos.x > g_fMapSizeX - fSizeHalfWidth)
    {
        m_vPos.x = g_fMapSizeX - fSizeHalfWidth;
    }
    if (m_vPos.y > g_fMapSizeY - fSizeHalfHeight)
    {
        m_vPos.y = g_fMapSizeY - fSizeHalfHeight;
    }
    // 충돌데이터 업데이트( 회전 처리 미반영 )
    TVector2 rt = { m_vPos.x, m_vPos.y };
    SetRect(rt, m_vScale.x * 2.0f, m_vScale.y * 2.0f);

    TMatrix matScale, matRotation, matTranslate;
    matScale.Scale(m_vScale);
    matRotation.ZRotate(m_vRotation.z);
    matTranslate.Translation(m_vPos);
    m_matWorld = matScale * matRotation * matTranslate;
    return true;
}
bool TPlayer::Init()
{
    return true;
}
bool TPlayer::Render()
{
    TPlaneObj::Render();
    return true;
}
bool TPlayer::Release()
{
    return true;
}