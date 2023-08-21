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