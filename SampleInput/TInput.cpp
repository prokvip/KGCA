#include "TInput.h"

bool TInput::Init()
{    
    ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
    return true;
}
// OR(논리합)
// b   0    1     a
// 0   0    1
// 1   1    1 
// AND(논리곱)
// b   0    1     a
// 0   0    0
// 1   0    1 
// sKey 1000 0000 0000 0000
//   &  1000 0000 0000 0000
DWORD  TInput::GetKey(DWORD dwKey)
{
    return m_dwKeyState[dwKey];
}
bool TInput::Frame()
{
    // 화면좌표
    GetCursorPos(&m_ptPos);
    // 클라이언트 좌표
    ScreenToClient(g_hWnd, &m_ptPos);

    for (int iKey = 0; iKey < 256; iKey++)
    {
        SHORT sKey = GetAsyncKeyState(iKey);
        if (sKey & 0x8000)
        {
            if (m_dwKeyState[iKey] == KEY_FREE)
            {
                m_dwKeyState[iKey] = KEY_PUSH;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_HOLD;
            }
        }
        else
        {
            if (m_dwKeyState[iKey] == KEY_PUSH ||
                m_dwKeyState[iKey] == KEY_HOLD )
            {
                m_dwKeyState[iKey] = KEY_UP;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_FREE;
            }
        }
    }
    return true;
}

bool TInput::Render()
{
    //TCHAR timerString[MAX_PATH] = { 0, };
    //_stprintf_s(timerString,
    //    L"\nx=%d:y%d",
    //    m_ptPos.x, m_ptPos.y);
    //OutputDebugString(timerString);
    return true;
}

bool TInput::Release()
{
    return true;
}
