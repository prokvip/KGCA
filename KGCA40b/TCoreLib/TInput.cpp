#include "TInput.h"
POINT g_ptMouse;

DWORD   TInput::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}
bool	TInput::Init()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256 );
	return true;
}
bool	TInput::Frame() 
{
	//화면(스크린)좌표계
	GetCursorPos(&m_ptMouse);
	//클라이언트(윈도우)좌표계
	ScreenToClient(g_hWnd, &m_ptMouse);
	g_ptMouse = m_ptMouse;

	// 마우스 버튼 VK_LBUTTON,  VK_RBUTTON, VK_MBUTTON,
	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);
		// 0000 0000 0000 0000
		// 1000 0000 0000 0000
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
				m_dwKeyState[iKey] == KEY_HOLD)
			{
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}

	m_dwMouseState[0] = m_dwKeyState[VK_LBUTTON];
	m_dwMouseState[1] = m_dwKeyState[VK_RBUTTON];
	m_dwMouseState[2] = m_dwKeyState[VK_MBUTTON];
	return true;
}
bool	TInput::Render() 
{
	
	return true;
}
bool	TInput::Release() {
	return true;
}
TInput::TInput()
{

}
TInput::~TInput()
{

}