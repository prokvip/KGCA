#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TServerStd.h"
#include "TPacket.h"

class TNetUser
{
public:
	bool		m_bConnect = false;
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	// buffer
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // 패킷의 시작주소
	int  m_iWritePos;  // 현재의 저장 위치
	int  m_iReadPos;
	std::list<TPacket> m_packetPool;
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

