#include "TSessionMgr.h"
#include "TServerIOCP.h"
#include "TServer.h"
tGUID		TSessionMgr::g_tGuidNil;

void TSessionMgr::Create(CRuntimeClass* pRTCMyClass)
{
	for (int iQueue = 0; iQueue < MAX_SESSION_COUNT; iQueue++)
	{
		TObject* pObj = pRTCMyClass->CreateObject();
		m_QueueSession.Push((TServerObj*)pObj);
	}
}

TUser* TSessionMgr::CreateNewSession(SOCKET sock, SOCKADDR_IN addr)
{
	TUser* newSession = dynamic_cast<TUser*>(m_QueueSession.Pop());		
	ASSERT(newSession);
	
	Lock();
		newSession->SetSocket(sock, addr);
		Add(newSession);
	Unlock();	
	return newSession;
}
void TSessionMgr::RemoveSession(TUser* pSession)
{
	m_QueueSession.Push((TServerObj*)pSession);
}

void TSessionMgr::ReleaseAll()
{
	for (TUserIterator iter = m_SessionList.begin(); iter != m_SessionList.end(); iter++)
	{
		TUser* delUser = (TUser*)iter->second;
		if (delUser != nullptr)
		{
			shutdown(delUser->m_Socket, SD_BOTH);
			closesocket(delUser->m_Socket);
			RemoveSession(delUser);
		}
	}
	m_SessionList.clear();
}
bool TSessionMgr::DelUser(tGUID tGuid, tGUID& tCharGuid)
{
	Lock();
		TUser* delUser = nullptr;
		TUserIterator	delIter = m_SessionList.find(tGuid);
		if (delIter != m_SessionList.end())
		{
			delUser = (TUser*)delIter->second;
			if (delUser)
			{
				tCharGuid = delUser->m_GuidActiveCharacter;				
				delUser->Disconnect();
				RemoveSession(delUser);
			}
			m_SessionList.erase(delIter);
		}
	Unlock();
	if (tCharGuid == g_tGuidNil) return false;
	return true;
}

bool TSessionMgr::DelUser(SOCKET sock, tGUID& tGuid, tGUID& tCharGuid)
{	
	UuidCreateNil(&tGuid);
	TUser* delUser = Find(sock);
	bool bRet = false;
	if (delUser)
	{
		tGuid = delUser->m_tGuid;
		bRet = DelUser(delUser->m_tGuid, tCharGuid);
	}
	return (bRet) ? true : false;
}

bool  TSessionMgr::Add(TUser* pUser)
{
	tGUID uuid;
	tGUID uuidLogin;
	tGUID uuidCharacter;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);
	ret_val = ::UuidCreate(&uuidLogin);
	ret_val = ::UuidCreate(&uuidCharacter);

	// 로그인 검증 GUID
	pUser->m_LoginGuid = uuidLogin;
	// 유저 GUID
	pUser->m_tGuid = uuid;
	// 유저 디폴트 케릭터 GUID
	pUser->m_GuidActiveCharacter = uuidCharacter;

	TCharacter defaultChar;
	defaultChar.SetGuid(uuidCharacter);
	pUser->GetCharacterList().insert(make_pair(uuidCharacter, defaultChar));

	std::map<tGUID, TCharacter, GUIDComparer>::iterator iter;
	iter = pUser->GetCharacterList().find(uuidCharacter);
	if (iter != pUser->GetCharacterList().end())
	{
		pUser->SetActiveCharacter((TCharacter*)&iter->second, true);
	}

	m_SessionList.insert(make_pair(pUser->m_tGuid, pUser));
	I_ServerIOCP.AddhandleToIOCP((HANDLE)pUser->m_Socket, (ULONG_PTR)pUser);
	pUser->WaitForZeroByteReceive();
	return true;
}
TUser* TSessionMgr::Find(SOCKET sock)
{
	Lock();
		TUserIterator	iter;
		TUser* pFindUser = nullptr;
		for (iter = m_SessionList.begin(); iter != m_SessionList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (pUser->m_Socket == sock)
			{
				pFindUser = pUser;
			}
		}
	Unlock();
	return pFindUser;
}

void TSessionMgr::Lock()
{
	if (InterlockedExchange(&m_iLockCheck, 1) != 0)
	{
		//T_LOG("%s%d\r\n", "DeadLock!! TSeeesionMgr ", m_iLockCheck);
	}
	WaitForSingleObject(m_hMutex, INFINITE);	
}
void  TSessionMgr::Unlock()
{
	ReleaseMutex(m_hMutex);
	InterlockedExchange(&m_iLockCheck, 0);
}
TUserList& TSessionMgr::GetSessionList()
{
	return m_SessionList;
}
TSessionMgr::TSessionMgr()
{
	m_iLockCheck = 0;
	m_hMutex = CreateMutex(NULL, FALSE, _T("TSessionMgr"));
	UuidCreateNil(&g_tGuidNil);
}
TSessionMgr::~TSessionMgr()
{
	m_SessionList.clear();
	while (m_QueueSession.m_List.size() > 0)
	{
		TUser* pSession = (TUser*)m_QueueSession.Pop();
		pSession->Destory();
		delete pSession;
	}
	CloseHandle(m_hMutex);
	m_hMutex = 0;
}
