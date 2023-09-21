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
   
    // 충돌데이터 업데이트( 회전 처리 미반영 )
    TVector2 rt = { m_vPos.x, m_vPos.y };
    SetRect(rt, m_vScale.x * 2.0f, m_vScale.y * 2.0f);

    TBASIS_EX::TMatrix matScale, matRotation, matTranslate;
    D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
    D3DXMatrixRotationZ(&matRotation, m_vRotation.z);
    D3DXMatrixTranslation(&matTranslate, m_vPos.x, m_vPos.y, m_vPos.z);

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