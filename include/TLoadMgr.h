#pragma once
#include <windows.h>

class TCallBackBase
{
public:
	virtual bool Execute() const = 0;
	virtual bool FinishExecute() const = 0;
};

template <class TObjectType>
class TLoadCallBack : public TCallBackBase
{
public:
	TLoadCallBack()
	{
		m_pFinishFunction=m_pFunction = 0;
		m_hThread = 0;
	}

	typedef bool (TObjectType::*FUNCTIONCALL)();

	virtual bool Execute() const
	{
		if (m_pFunction) return (m_pObjectType->*m_pFunction)();
		return false;
	}
	virtual bool FinishExecute() const
	{
		if (m_pFinishFunction) return (m_pFinishObjectType->*m_pFinishFunction)();
		return false;
	}
	void SetCallback(TObjectType	*pObjectType,
		FUNCTIONCALL   pFunction)
	{
		m_pObjectType = pObjectType;
		m_pFunction = pFunction;
	}
	void SetFinishCallback(TObjectType	*pObjectType,
		FUNCTIONCALL   pFunction)
	{
		m_pFinishObjectType = pObjectType;
		m_pFinishFunction = pFunction;
	}
public:
	static DWORD	WINAPI LoadMgrThread(void* lParam);

	DWORD	m_dwThreadID;
	HANDLE	m_hThread;
	bool	m_bEndOfLoadData;
	HANDLE  m_hFinishEvent;

public:
	bool	NewThread();
	bool	SetThread();
	bool	EndThread();
	bool  TerminateThread();
	bool  IsRunThread();
	bool	FiniteWaitThread();
private:
	TObjectType  *m_pObjectType;
	FUNCTIONCALL  m_pFunction;
private:
	TObjectType  *m_pFinishObjectType;
	FUNCTIONCALL  m_pFinishFunction;

};

template <class TObjectType >
TLoadCallBack<TObjectType> SetProc(TObjectType pObjType)
{
	return TLoadCallBack<TObjectType>(&pObjType);
}
template <class TObjectType>
DWORD  WINAPI TLoadCallBack<TObjectType>::LoadMgrThread(void* lParam)
{
	if (lParam == NULL) return 1;
	TLoadCallBack* pLoadMgr = (TLoadCallBack*)lParam;
	if (pLoadMgr == NULL) return 1;
	pLoadMgr->Execute();
	// 작업완료 통지 함수 호출
	//pLoadMgr->FinishExecute();
	SetEvent(pLoadMgr->m_hFinishEvent);
	return 0;
}

template <class TObjectType>
bool TLoadCallBack<TObjectType>::NewThread()
{
	m_bEndOfLoadData = false;
	m_hThread = CreateThread(NULL, 0, TLoadCallBack::LoadMgrThread, this, CREATE_SUSPENDED, &m_dwThreadID);
	if (m_hThread == NULL)
	{
		return false;
	}
	// 2 : FALSE(자동리셋 이벤트 ),TRUE( 수동리셋 이벤트 )
	// 3 : 초기값(FALSE: 논시그널, TRUE:시그널)
	m_hFinishEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	return true;
}
template <class TObjectType>
bool TLoadCallBack<TObjectType>::IsRunThread()
{
	if (m_bEndOfLoadData == true) return true;
	DWORD dwExitCode;
	GetExitCodeThread(m_hThread, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
	{
		return false;
	}
	m_bEndOfLoadData = true;
	// 작업완료 통지 함수 호출
	FinishExecute();
	return true;
}
// 쓰레드 종료 무한 대기 후 콜백함수 호출
template <class TObjectType>
bool TLoadCallBack<TObjectType>::FiniteWaitThread()
{
	if (m_bEndOfLoadData == true) return true;
	DWORD dwRet = WaitForSingleObject(m_hThread, INFINITE);
	if (dwRet != WAIT_OBJECT_0)
	{
		return false; // error
	}
	m_bEndOfLoadData = true;
	// 작업완료 통지 함수 호출
	FinishExecute();
	return true;
}

template <class TObjectType>
bool TLoadCallBack<TObjectType>::SetThread()
{
	ResumeThread(m_hThread);
	return true;
}

template <class TObjectType>
bool TLoadCallBack<TObjectType>::EndThread()
{
	DWORD dwExitCode;
	if (GetExitCodeThread(m_hThread, &dwExitCode) != 0)
	{
		ExitThread(dwExitCode);
		CloseHandle(m_hThread);
	}
	//TerminateThread(m_hThread, m_dwThreadID);
	//CloseHandle(m_hThread);

	// 작업완료 통지 함수 호출
	FinishExecute();
	return true;
}
template <class TObjectType>
bool TLoadCallBack<TObjectType>::TerminateThread()
{
	DWORD dwExitCode;
	GetExitCodeThread(m_hThread, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
	{
		::TerminateThread(m_hThread, dwExitCode);
		CloseHandle(m_hThread);
	}
	// 작업완료 통지 함수 호출
	FinishExecute();
	return true;
}

