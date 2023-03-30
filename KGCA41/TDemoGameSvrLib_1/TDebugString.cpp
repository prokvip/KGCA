#include "TDebugString.h"
#include "TLock.h"
#include "TServer.h"

#define _SECOND 10000000
// 대기가능타이머
typedef struct _MYDATA { TCHAR *szText; DWORD dwValue; } MYDATA;

VOID CALLBACK TimerAPCProc(
	LPVOID lpArg,               // Data value
	DWORD dwTimerLowValue,      // Timer low value
	DWORD dwTimerHighValue)    // Timer high value

{
	// Formal parameters not used in this example.
	UNREFERENCED_PARAMETER(dwTimerLowValue);
	UNREFERENCED_PARAMETER(dwTimerHighValue);
	MYDATA *pMyData = (MYDATA *)lpArg;
	_tprintf(TEXT("Message: %s\nValue: %d\n\n"), pMyData->szText, pMyData->dwValue);
	T_LOG("Message: %s\nValue: %d\r\n", pMyData->szText, pMyData->dwValue);
	MessageBeep(0);
}
//https://docs.microsoft.com/en-us/windows/desktop/sync/using-a-waitable-timer-with-an-asynchronous-procedure-call

void CALLBACK TimerProc(void* lpParametar, BOOLEAN TimerOrWaitFired)
{
	TDebugString* obj = (TDebugString*)lpParametar;
	obj->QueueTimerHandler();
}
void TDebugString::QueueTimerHandler() 
{
	//T_LOG("Message: %s : Value: %d\r\n", "test", I_Server.m_dwSendPacket, I_Server.m_dwRecvPacket);
	char szBuffer[256];
	sprintf_s(szBuffer, "%d:%d", I_Server.m_dwSendPacket, I_Server.m_dwRecvPacket);
	SetWindowTextA(m_hWnd, szBuffer);
	InterlockedExchange(&I_Server.m_dwSendPacket, 0);
	InterlockedExchange(&I_Server.m_dwRecvPacket, 0);
}
void TDebugString::Delay(DWORD dwTime)
{
	BOOL            bSuccess;
	__int64         qwDueTime;
	LARGE_INTEGER   liDueTime;
	MYDATA          MyData;

	MyData.szText = TEXT("This is my data");
	MyData.dwValue = 100;

	HANDLE hTimer = CreateWaitableTimer(
		NULL,                   // Default security attributes
		FALSE,                  // Create auto-reset timer
		TEXT("MyTimer"));       // Name of waitable timer
	if (hTimer != NULL)
	{
		__try
		{
			// Create an integer that will be used to signal the timer 
			// 5 seconds from now.
			qwDueTime = -dwTime * _SECOND;

			// Copy the relative time into a LARGE_INTEGER.
			liDueTime.LowPart = (DWORD)(qwDueTime & 0xFFFFFFFF);
			liDueTime.HighPart = (LONG)(qwDueTime >> 32);

			bSuccess = SetWaitableTimer(
				hTimer,           // Handle to the timer object
				&liDueTime,       // When timer will become signaled
				2000,             // Periodic timer interval of 2 seconds
				TimerAPCProc,     // Completion routine
				&MyData,          // Argument to the completion routine
				FALSE);          // Do not restore a suspended system

			if (bSuccess)
			{
				SleepEx(INFINITE,  TRUE);
			}
			else
			{
				printf("SetWaitableTimer failed with error %d\n", GetLastError());
			}

		}
		__finally
		{
			CloseHandle(hTimer);
		}
	}
}

void TDebugString::ErrorMsg()
{
	if( WSAGetLastError() != WSA_IO_PENDING )
	{
		setlocale(LC_ALL,"KOREAN");
		char* lpMsgBuf;
		FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER|
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);

		char szBuffer[256];															
		sprintf_s(szBuffer, ("[File: %s][Line: %d]\n[Note : %s]"), __FILE__, __LINE__, lpMsgBuf);
		DisplayText("ERROR:%s\r\n", (char*)szBuffer);					
		LocalFree(lpMsgBuf);		
	}
}

HRESULT TDebugString::ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
										  int cchDestChar )
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
		cchDestChar*sizeof(CHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}
TCHAR* TDebugString::GetMbtoWcs( const char* srcmsg )
{
	// 멀티바이트 => 유니코드 변환.
	TCHAR msg[1024] = {0, };
	ConvertAnsiStringToWideCch(msg, srcmsg, 1024);
	return msg;
}
HRESULT TDebugString::ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
									const CHAR* strSource, 
									int cchDestChar )
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
										wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}
char* TDebugString::GetWcstoMbs( WCHAR* srcmsg )
{
	// 멀티바이트 => 유니코드 변환.
	char msg[1024] = {0, };
	ConvertAnsiStringToWideCch(srcmsg, msg, 1024);
	return msg;
}

void TDebugString::DisplayText( char* fmt, ...)
{	
	{
		TLock sync( this );		
		va_list arg;
		va_start(arg, fmt );	
			char buf[1024+256] = {0,};
			vsprintf_s( buf, fmt, arg );	
			int nLen=GetWindowTextLength(m_hEdit); 
			if( nLen > 3000 )
			{
				SendMessage( m_hEdit, EM_SETSEL, 0, -1);            
				SendMessage( m_hEdit, WM_CLEAR, 0, 0);
			}
			SendMessage( m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)GetMbtoWcs(buf) );
			OutputDebugStringA((char*)buf);
		va_end(arg );		
	}
}
bool TDebugString::Init()
{
	RECT rt;
	GetClientRect(m_hWnd, &rt);
	// 에디터 컨트롤 윈도우 생성
	m_hEdit = CreateWindow(	 L"edit", NULL,
							WS_CHILD | WS_VISIBLE |WS_HSCROLL|WS_VSCROLL 
							| ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY
							| ES_AUTOHSCROLL,
							rt.left, rt.top,
							rt.right - rt.left,
							rt.bottom - rt.top,
		m_hWnd,NULL,
		m_hInst,
							NULL );
	m_hTimerQueue = CreateTimerQueue();
	BOOL success = ::CreateTimerQueueTimer(
		&m_hTimer,
		m_hTimerQueue,
		TimerProc,
		this,
		0,
		1000,
		WT_EXECUTEINTIMERTHREAD);

	return true;
}
bool TDebugString::Frame()
{		
	return true;
}
bool TDebugString::Release()
{
	DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL);
	DeleteTimerQueueEx(m_hTimerQueue, INVALID_HANDLE_VALUE);
	return true;
}
TDebugString::TDebugString(void)
{
	m_dwSendPacket = 0;
}

TDebugString::~TDebugString(void)
{
}
