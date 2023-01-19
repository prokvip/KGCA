#pragma once
#include "TSessionUser.h"
#include "TObjectPool.h"
class TSessionMgr
{
	TClassPool<TSessionUser>  m_Pool;
public:
	std::list<std::shared_ptr<TSessionUser>> m_SessionList;
	TSessionUser* Add(SOCKET sock, SOCKADDR_IN address);
	void   Delete(std::shared_ptr<TSessionUser> user);
	std::list<UPACKET>  m_BroadcasttingPacketList;
	void   SendPrecess();
	TSessionMgr() : m_Pool(20)
	{
		OVERLAPPED2::Allocation();
	}
	~TSessionMgr()
	{
		OVERLAPPED2::Release();
	}
};

