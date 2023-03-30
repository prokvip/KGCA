// Fill out your copyright notice in the Description page of Project Settings.

#include "TStreamPacket.h"
#include <assert.h>

void   TStreamPacket::Put(char* recvBuffer, int iRecvSize)
{
	EnterCriticalSection(&m_CS);
	{
		// WSARecv ���۰� 1���� ��Ŷ ũ�⺸�� ����� ũ�� ��Ŷ�� �񵿱� ó���� 1���� ���´�.
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
	
		// ������  ���� ��Ŷ�� ũ�⺸�� WSARecv ���۰� ������ �ɰ����� ���´�.
		// �޴� ������ �뷮�� �����ϸ�
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

		// ��Ŷ�� ����
		m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
		if (m_pPacket->ph.iotype != 0x0309)
		{
			LeaveCriticalSection(&m_CS);
			return;
		}
		// 1���� ��Ŷ �����ŭ �޾Ҵٸ�
		if (m_iReadPos >= m_pPacket->ph.len)
		{
			do {
				UPACKET    add;
				ZeroMemory(&add, sizeof(add));
				CopyMemory(&add, &m_RecvBuffer[m_iStartPos], m_pPacket->ph.len);
				m_PacketList.push_back(add);			

				// 1���� ��Ŷ�� ó���ϰ� ���� �ܿ��� ũ��
				m_iReadPos -= m_pPacket->ph.len;
				// ��Ŷ�� ���� ��ġ
				m_iStartPos += m_pPacket->ph.len;

				// �ܿ����� ��Ŷ ������ ���� ���
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



