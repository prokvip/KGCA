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

    float fHalfWidth  = g_fMapSizeX / 2.0f;
    float fHalfHeight = g_fMapSizeY / 2.0f;
    float fSizeHalfWidth = m_vScale.x;
    float fSizeHalfHeight = m_vScale.y;
    if (m_vPos.x < -fHalfWidth + fSizeHalfWidth)
    {
        m_vPos.x = -fHalfWidth + fSizeHalfWidth;
    }
    if (m_vPos.y < -fHalfHeight+ fSizeHalfHeight)
    {
        m_vPos.y = -fHalfHeight + fSizeHalfHeight;
    }
    if (m_vPos.x > fHalfWidth - fSizeHalfWidth)
    {
        m_vPos.x = fHalfWidth - fSizeHalfWidth;
    }
    if (m_vPos.y > fHalfHeight - fSizeHalfHeight)
    {
        m_vPos.y = fHalfHeight - fSizeHalfHeight;
    }

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