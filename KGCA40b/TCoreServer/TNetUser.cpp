#include "TNetUser.h"
#include "TServer.h"

int TNetUser::Recv()
{
	// 비동기 로드	
	m_wsaRecvBuffer.len = sizeof(char)*256;
	m_wsaRecvBuffer.buf = m_szRecv;
	m_ovRecv.type = 1000;
	DWORD dwRead;
	DWORD lpFlags=0;
	BOOL ret = WSARecv(m_Sock, 
		&m_wsaRecvBuffer,
		1, 
		&dwRead,
		&lpFlags,
		(WSAOVERLAPPED*)&m_ovRecv,
		nullptr);
	return 0;
}
int TNetUser::SendMsg(char* msg, int iSize, WORD type)
{
	// 비동기 로드	
	UPACKET uPacket;
	uPacket.ph.len = iSize +PACKET_HEADER_SIZE;
	uPacket.ph.type = type;
	memcpy(uPacket.msg, msg, iSize);
	//memcpy(m_szSend, &uPacket, uPacket.ph.len);

	m_wsaSendBuffer.len = uPacket.ph.len;
	m_wsaSendBuffer.buf = (char*)&uPacket;
	m_ovSend.type = 2000;
	DWORD dwWrite;
	DWORD lpFlags = 0;
	BOOL ret = WSASend(m_Sock,
		&m_wsaSendBuffer,
		1,
		&dwWrite,
		0,
		(WSAOVERLAPPED*)&m_ovSend,
		nullptr);
	return 0;
}
int TNetUser::SendMsg(UPACKET& packet)
{
	// 비동기 로드	
	m_wsaSendBuffer.len = packet.ph.len;
	m_wsaSendBuffer.buf = (char*)&packet;
	memcpy(m_wsaSendBuffer.buf, (char*)&packet, packet.ph.len- PACKET_HEADER_SIZE);
	m_ovSend.type = 2000;
	DWORD dwWrite;
	DWORD lpFlags = 0;
	BOOL ret = WSASend(m_Sock,
		&m_wsaSendBuffer,
		1,
		&dwWrite,
		0,
		(WSAOVERLAPPED*)&m_ovSend,
		nullptr);
	return 0;
}
int TNetUser::Dispatch(DWORD dwTrans, TOV* tov)
{
	if (m_bConnect == false)
	{
		return 0;
	}
	if (tov->type == 1000)
	{
		if (!DispatchRecv(m_szRecv, dwTrans))
		{
		}	
		Recv();
	}
	if(tov->type == 2000)
	{
		if (!DispatchSend(dwTrans))
		{
		}
	}
	return 1;
}
int TNetUser::DispatchRecv(char* szRecvBuffer, int iRecvByte)
{
	//p1(m_iPacketPos)  p2(2.1)       pn   (m_ReadPos)
	//2035 ~ 2038 ~ 22  ~ 50  ~  2028 ~ 2038 ~ 2048 
	//0 ~ 5 ~ iRecvByte
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;// 버퍼에 이전에 저장된 위치
	m_iReadPos += iRecvByte; // 패킷시작 위치로부터 받은 바이트

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// 적어도 1개의 패킷은 도착했다.
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				TPacket tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);
				if (pPacket->ph.type == PACKET_CHAT_MSG)
				{
					m_packetPool.push_back(tPacket);
				}
				else
				{
					XPacket xPacket;
					xPacket.pUser = this;
					xPacket.packet = tPacket;
					m_pServer->m_packetPool.push_back(xPacket);
				}

				// 다음패킷 처리
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);
		}
	}
	return 1;
}
int TNetUser::DispatchSend(DWORD dwTrans)
{
	return 0;
}
void TNetUser::set(SOCKET sock, SOCKADDR_IN addr,
	TServer* pServer)
{
	m_pServer = pServer;
	m_bConnect = true;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}
bool TNetUser::DisConnect()
{
	closesocket(m_Sock);
	return true;
}

TNetUser::TNetUser()
{

}
TNetUser::~TNetUser()
{

}
