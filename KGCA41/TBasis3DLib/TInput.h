#pragma once
#include "TBasisStd.h"
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")

#define KEYSTATECOUNT 256
//#define WIN_KEY_USE

enum KeyState {
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_PUSH = 2,	
	KEY_HOLD = 3,
};

class TInput : public TSingleton < TInput >
{
public:
	HWND   m_hWnd;
	//  ÀÎÇ² °´Ã¼
	LPDIRECTINPUT8		m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // Å°º¸µå
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// ¸¶¿ì½º
	BYTE				  m_KeyState[KEYSTATECOUNT];
	BYTE				  m_KeyStateOld[KEYSTATECOUNT];
	DIMOUSESTATE		  m_DIMouseState;
	std::vector<wstring>  m_strList;
	TCHAR		m_csBuffer[MAX_PATH];
	BYTE		m_MouseState[3];
	BYTE		m_BeforeMouseState[3];
	POINT		m_MousePos;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	void		MsgEvent(MSG msg);
	BYTE		KeyCheck(BYTE dwKey);
	bool		InitDirectInput();
	void		DeviceAcquire();
	void		DeviceUnacquire();
public:
	TInput();
	virtual ~TInput();
};
#define I_Input TInput::GetInstance()