#pragma once
#include "TServerObj.h"

class TDebugString: public TSingleton<TDebugString>, public TServerObj
{
private:
	friend class TSingleton<TDebugString>;
public:
	HANDLE          m_hTimer;
	HANDLE			m_hTimerQueue;
public:
	TCHAR_STRING_VECTOR	m_Message;
	HWND			m_hEdit;
	HWND			m_hWnd;
	HINSTANCE		m_hInst;
	DWORD			m_dwSendPacket;
	void		Set(HWND hWnd, HINSTANCE hInst)
	{
		m_hWnd = hWnd;
		m_hInst = hInst;
	}
	void		DisplayText( char* fmt, ...);
	TCHAR*		GetMbtoWcs( const char* srcmsg );
	char*		GetWcstoMbs( WCHAR* srcmsg );
	HRESULT		ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
										const CHAR* strSource, 
										int cchDestChar );
	HRESULT		ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
										  int cchDestChar );
public:
	bool		Init();
	bool		Frame();
	bool		Release();
	void		ErrorMsg();
	void		Delay(DWORD dwTime);
	void		QueueTimerHandler();
public:
	TDebugString(void);
	~TDebugString(void);
};
#define I_DebugStr TDebugString::GetInstance()
#define T_LOG(s,...) I_DebugStr.DisplayText(s,##__VA_ARGS__);
#define T_ERR		I_DebugStr.ErrorMsg();
