#include "TInput.h"
TVector3 TInput::GetWorldPos( TVector2 vWindow, TVector3 vCamera)
{
    float fHalfWidth = vWindow.x / 2.0f;
    float fHalfHeight = vWindow.y / 2.0f;

    // client
    TVector3 vMouse = { (float)m_MousePos.x, (float)m_MousePos.y , 0.0f};
    // world
    TVector2 vMouseWorldLT = { vCamera.x - fHalfWidth,
                               vCamera.y + fHalfHeight };
    vMouse.x = vMouseWorldLT.x + vMouse.x;
    vMouse.y = vMouseWorldLT.y - vMouse.y;
    return vMouse;
}
bool  TInput::Frame()
{       
    ::GetCursorPos(&m_MousePos); // ½ºÅ©¸° ÁÂÇ¥
    ::ScreenToClient(g_hWnd, &m_MousePos);

    m_vOffset.x = m_MousePos.x - m_BeforeMousePos.x;
    m_vOffset.y = m_MousePos.y - m_BeforeMousePos.y;

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

    m_BeforeMousePos = m_MousePos;
    return true;
}
bool  TInput::Init()
{
    ZeroMemory(&m_dwKeyState, sizeof(m_dwKeyState));
    RECT rcClip;           // new area for ClipCursor
    RECT rcOldClip;        // previous area for ClipCursor

    // Record the area in which the cursor can move. 

    GetClipCursor(&rcOldClip);

    // Get the dimensions of the application's window. 

    GetWindowRect(g_hWnd, &rcClip);

    // Confine the cursor to the application's window. 

    ClipCursor(&rcClip);

    // 
    // Process input from the confined cursor. 
    // 

 // Restore the cursor to its previous area. 

    //ClipCursor(&rcOldClip);
    return true;
}
bool  TInput::Render()
{
#ifdef _DEBUG
    std::wstring mousePos = std::to_wstring(m_MousePos.x);
    mousePos += L",";
    mousePos += std::to_wstring(m_MousePos.y);
    mousePos += L"\n";
    //T_Debug(mousePos.c_str());
#endif
    return true;
}
bool  TInput::Release()
{

    return true;
}