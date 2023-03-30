#pragma once
#include "TUser.h"
#include "TPacketPool.h"
#include "TQueue.h"

#define MAX_SESSION_COUNT 1000

typedef std::map<tGUID, TCharacter, GUIDComparer>::iterator	TCharacterIterator;
typedef std::map<tGUID, TUser*, GUIDComparer>::iterator	TUserIterator;
typedef std::map<tGUID, TUser*, GUIDComparer> TUserList;

class TSessionMgr : public TSingleton<TSessionMgr>
{
private:
	HANDLE				m_hMutex;
	TUserList			m_SessionList;
	static tGUID		g_tGuidNil;
private:
	friend class TSingleton<TSessionMgr>;
	TQueue					m_QueueSession;
	volatile long 			m_iLockCheck;
public:
	void	Create(CRuntimeClass* pRTCMyClass = NULL);
	TUser*  CreateNewSession(SOCKET sock, SOCKADDR_IN addr);
private:
	bool	Add(TUser* pUser);	
	void	RemoveSession(TUser* pSession);
public:
	void	Lock();
	void	Unlock();
	TUserList& GetSessionList();
	TUser*	Find(SOCKET iIndex);
	bool	DelUser(tGUID tGuid, tGUID& tCharGuid);
	bool	DelUser(SOCKET iIndex, tGUID& tGuid, tGUID& tCharGuid);
	void	ReleaseAll();
public:
	virtual ~TSessionMgr();
private:
	TSessionMgr();
};
#define ISessionMgr  TSessionMgr::GetInstance()
