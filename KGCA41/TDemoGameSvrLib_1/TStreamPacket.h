#pragma once
#include "TObjectPool.h"
#include "TProtocol.h"
#define MAX_RECV_STREAM_SIZE 2048

class TUser;

// 어떤 메세지인지 구분해야 한다. ( 필요성 )
struct OVERLAPPED2 : ObjectPool<OVERLAPPED2>//: OVERLAPPED//, ObjectPool<OVERLAPPED2>
{	
	enum { MODE_RECV = 0, MODE_SEND = 1, MODE_ZERO_SEND, MODE_ZERO_RECV, };
	OVERLAPPED ov;
	int		m_iFlags;
	OVERLAPPED2(int iMode)
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		m_iFlags = iMode;
	}
	OVERLAPPED2()
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		m_iFlags = MODE_RECV;
	}
};

typedef struct {
	UPACKET			packet;
	WSABUF			buffer;
	OVERLAPPED2*	ov;
	SOCKET          sock;
	TUser*			pUser;
}T_PACKET, *P_TPACKET;

typedef std::list<T_PACKET>::iterator TPacketIterator;

class TStreamPacket : public TServerObj
{
public:	
	UPACKET*    m_pPacketStart;
	PVOID       m_pWritePos;
	PVOID 		m_pPacket;
	char		m_RecvBuffer[MAX_RECV_STREAM_SIZE];
	long long volatile	m_iStartPos;
	long long volatile	m_iWritePos;
	long long volatile	m_iReadPos;
public:
	bool    Put(char* redvBuffer, int iSize, TUser* = NULL);
	bool    GetPacket(TUser* pUser);
	bool	ParityCheck(TUser* pUser);
	void    Reset();
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};
