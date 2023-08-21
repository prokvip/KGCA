#include "TInput.h"
bool  TInput::Frame()
{       
    ::GetCursorPos(&m_MousePos); // 스크린 좌표
    ::ScreenToClient(g_hWnd, &m_MousePos);

    for (int ikey = 0; ikey < 256; ikey++)
    {
        SHORT s = GetAsyncKeyState(ikey);
        if (s & 0x8000) // 1000 0000 0000 0000
        {
            if(m_dwKeyState[ikey] == KEY_FREE)
                m_dwKeyState[ikey] = KEY_PUSH;
            else
                m_dwKeyState[ikey] = KEY_HOLD;
        }
        else
        {
            if (m_dwKeyState[ikey] == KEY_PUSH ||
                m_dwKeyState[ikey] == KEY_HOLD)
                 m_dwKeyState[ikey] = KEY_UP;
            else
                m_dwKeyState[ikey] = KEY_FREE;
        }
    }    
    return true;
}
bool  TInput::Init()
{
    ZeroMemory(&m_dwKeyState, sizeof(m_dwKeyState));
    return true;
}
bool  TInput::Render()
{
#ifdef _DEBUG
    std::wstring mousePos = std::to_wstring(m_MousePos.x);
    mousePos += L",";
    mousePos += std::to_wstring(m_MousePos.y);
    mousePos += L"\n";
    T_DebugString(mousePos.c_str());
#endif
    return true;
}
bool  TInput::Release()
{

    return true;
}