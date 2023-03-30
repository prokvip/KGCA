// Fill out your copyright notice in the Description page of Project Settings.

#include "TStreamPacket.h"
#include <assert.h>

void   TStreamPacket::Put(char* recvBuffer, int iRecvSize)
{
	EnterCriticalSection(&m_CS);
	{
		// WSARecv 버퍼가 1개의 패킷 크기보다 충분히 크면 패킷은 비동기 처리로 1개씩 들어온다.
		m_pPacket = (P_UPACKET)recvBuffer;
		if (m_pPacket->ph.iotype == 0x0309 &&
			m_pPacket->ph.len == iRecvSize)
		{
			UPACKET    add;
			ZeroMemory(&add, sizeof(add));
			CopyMemory(&add, recvBuffer, m_pPacket->ph.len);
			m_PacketList.push_back(add);
			LeaveCriticalSection(&m_CS);
			return;
		}
	
		// 하지만  보낸 패킷의 크기보다 WSARecv 버퍼가 작으면 쪼개져서 들어온다.
		// 받는 버퍼의 용량이 부족하면
		if (m_iWritePos + iRecvSize >= MAX_RECV_SIZE)
		{
			MoveMemory(m_RecvBuffer, &m_RecvBuffer[m_iStartPos], m_iReadPos);
			m_iStartPos = 0;
			m_iWritePos = m_iReadPos;
		}

		CopyMemory(&m_RecvBuffer[m_iWritePos], recvBuffer, iRecvSize);
		m_iWritePos += iRecvSize;
		m_iReadPos += iRecvSize;

		if (m_iReadPos < PACKET_HEADER_SIZE)
		{
			LeaveCriticalSection(&m_CS);
			return;
		}

		// 패킷의 시작
		m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
		if (m_pPacket->ph.iotype != 0x0309)
		{
			LeaveCriticalSection(&m_CS);
			return;
		}
		// 1개의 패킷 사이즈만큼 받았다면
		if (m_iReadPos >= m_pPacket->ph.len)
		{
			do {
				UPACKET    add;
				ZeroMemory(&add, sizeof(add));
				CopyMemory(&add, &m_RecvBuffer[m_iStartPos], m_pPacket->ph.len);
				m_PacketList.push_back(add);			

				// 1개의 패킷을 처리하고 남은 잔여량 크기
				m_iReadPos -= m_pPacket->ph.len;
				// 패킷의 시작 위치
				m_iStartPos += m_pPacket->ph.len;

				// 잔여량이 패킷 헤더모다 작을 경우
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
				if (m_pPacket->ph.iotype != 0x0309)
				{
					LeaveCriticalSection(&m_CS);
					return;
				}

			} while (m_iReadPos >= m_pPacket->ph.len);
		}
		
	}
	LeaveCriticalSection(&m_CS);
};
void TStreamPacket::Reset()
{
	ZeroMemory(m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
	m_iStartPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_pPacket = NULL;
}
TStreamPacket::TStreamPacket() 
{
	Reset();
	InitializeCriticalSection(&m_CS);
}
TStreamPacket::~TStreamPacket()
{
	DeleteCriticalSection(&m_CS);
}



