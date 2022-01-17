#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TServerObj.h"
#include "TPacket.h"
struct TOV 
{
	OVERLAPPED ov;
	int  type;	
};

class TNetUser : public TServerObj
{
public:
	bool		m_bConnect = false;
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	// buffer
	TOV			m_ovRecv;
	TOV			m_ovSend;
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
	void set(SOCKET sock, SOCKADDR_IN addr);
	int  Recv();
	bool DisConnect();
public:
	TNetUser();
	virtual ~TNetUser();
};

