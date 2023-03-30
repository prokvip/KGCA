#pragma once
#include "TStreamPacket.h"

class TPacketPool
{
private:
	HANDLE				m_hMutex;
	std::list<T_PACKET> m_PacketList;	
	volatile long 		m_iLockCheck;
public:
	int					m_iType;
public:
	void  AddPacket(T_PACKET pack);
	void  Lock();
	void  Unlock();
	void  Clear();
	std::list<T_PACKET>& Get();
public:
	TPacketPool();
	virtual ~TPacketPool();
};

