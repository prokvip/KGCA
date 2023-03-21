#include "pch.h"
#include "TInput.h"
bool		TInput::Init()
{
    ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
    ::GetCursorPos(&m_ptPos); // ȭ����ǥ
    ::ScreenToClient(g_hWnd, &m_ptPos); // Ŭ���̾�Ʈ
    m_ptPrePos = m_ptPos;
    return true;
}
DWORD  TInput::GetKey(DWORD dwKey)
{
    return m_dwKeyState[dwKey];
}
bool		TInput::Frame()
{
    ::GetCursorPos(&m_ptPos); // ȭ����ǥ
    ::ScreenToClient(g_hWnd, &m_ptPos); // Ŭ���̾�Ʈ
    m_ptOffset.x = m_ptPos.x - m_ptPrePos.x;
    m_ptOffset.y = m_ptPos.y - m_ptPrePos.y;

    for (int iKey = 0; iKey < 256; iKey++)
    {
        SHORT sKey = ::GetAsyncKeyState(iKey); // �񵿱� Ű ���� 
        if (sKey & 0x8000) // 1000 0000 0000 0000
        {
            if (m_dwKeyState[iKey] == KEY_FREE ||
                m_dwKeyState[iKey] == KEY_UP)
                m_dwKeyState[iKey] = KEY_PUSH;
            else
                m_dwKeyState[iKey] = KEY_HOLD;
        }
        else
        {
            if (m_dwKeyState[iKey] == KEY_PUSH ||
                m_dwKeyState[iKey] == KEY_HOLD)
                m_dwKeyState[iKey] = KEY_UP;
            else
                m_dwKeyState[iKey] = KEY_FREE;
        }
    }
    m_ptPrePos = m_ptPos;
    return true;
}
bool		TInput::Render()
{
    return true;
}
bool		TInput::Release()
{
    return true;
}