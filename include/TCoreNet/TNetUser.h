#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TNetStd.h"
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
	int  m_iPacketPos; // ��Ŷ�� �����ּ�
	int  m_iWritePos;  // ������ ���� ��ġ
	int  m_iReadPos;
	std::list<TPacket> m_packetPool;
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

