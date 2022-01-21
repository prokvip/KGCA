#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TServerObj.h"
#include "TPacket.h"
#include "TObjectPool.h"
class TServer;
struct TOV : public TObjectPool<TOV>
{
	enum { MODE_RECV=1, MODE_SEND=2, MODE_EXIT};
	OVERLAPPED ov;
	int  type;	
	TOV(int packetType)
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		type = packetType;
	}
	TOV()
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		type = MODE_RECV;
	}
};

class TNetUser : public TServerObj, public TObjectPool<TNetUser>
{
	TServer* m_pServer=nullptr;
public:
	bool		m_bConnect = false;
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	WSABUF		m_wsaRecvBuffer;
	WSABUF		m_wsaSendBuffer;
	char		m_szRecv[256];
	char		m_szSend[256];
public:
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // 패킷의 시작주소
	int  m_iWritePos;  // 현재의 저장 위치
	int  m_iReadPos;
	std::list<TPacket> m_packetPool;
	int Dispatch(DWORD dwTrans, TOV* tov);	
	int DispatchRecv(char* szRecvBuffer, int iRecvByte);
	int DispatchSend(DWORD dwTrans);
	void set(SOCKET sock, SOCKADDR_IN addr, TServer* pServer);
	int  Recv();
	int  SendMsg(char* msg, int iSize, WORD type);
	int  SendMsg(UPACKET& packet);
	bool DisConnect();
public:
	TNetUser();
	virtual ~TNetUser();
};

