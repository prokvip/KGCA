#include "TPacketPool.h"
#include "TDebugString.h"

void  TPacketPool::AddPacket(T_PACKET pack)
{
	Lock();
		m_PacketList.push_back(pack);
	Unlock();
}
void  TPacketPool::Clear()
{
	Lock();
		m_PacketList.clear();
	Unlock();
}
void TPacketPool::Lock()
{
	if (InterlockedExchange(&m_iLockCheck, 1) != 0)
	{
	//	T_LOG("%s\r\n", "DeadLock!! PacketPool");
	}
	WaitForSingleObject(m_hMutex, INFINITE);
}
void  TPacketPool::Unlock()
{	
	ReleaseMutex(m_hMutex);
	InterlockedExchange(&m_iLockCheck, 0);
}
std::list<T_PACKET>& TPacketPool::Get()
{
	return m_PacketList;
}
TPacketPool::TPacketPool()
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL );
	m_iLockCheck = 0;
	m_iType = 0;
}


TPacketPool::~TPacketPool()
{
	CloseHandle(m_hMutex);
	m_hMutex = 0;
}
