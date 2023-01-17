#include "TSessionUser.h"
bool TSessionUser::Set(SOCKET sock, SOCKADDR_IN address)
{
	this->m_Sock = sock;
	this->m_Address = address;

    RecvMsg();
	return true;
}
int     TSessionUser::SendMsg(UPACKET& packet)
{
    m_wsaSendBuffer.buf = (char*)&packet;
    m_wsaSendBuffer.len = packet.ph.len;

    ZeroMemory(&m_SendOV, sizeof(OVERLAPPED2));
    m_SendOV.iType = OVERLAPPED2::MODE_SEND;

    DWORD dwSendBytes;// = send(sock, msgSend, packet.ph.len, 0);
    int iRet = WSASend(m_Sock, &m_wsaSendBuffer, 1, &dwSendBytes, 0, &m_SendOV, NULL);

    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            m_bDisConnect = true;
            return -1;
        }
    }
    return 1;
}
int   TSessionUser::SendMsg(short type, char* msg)
{
    UPACKET packet;
    ZeroMemory(&packet, sizeof(UPACKET));
    if (msg != nullptr)
    {
        packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
        memcpy(packet.msg, msg, strlen(msg));
    }
    else
    {
        packet.ph.len = PACKET_HEADER_SIZE;
    }
    packet.ph.type = type;

    m_wsaSendBuffer.buf = (char*)&packet;
    m_wsaSendBuffer.len = packet.ph.len;
    
    ZeroMemory(&m_SendOV, sizeof(OVERLAPPED2));
    m_SendOV.iType = OVERLAPPED2::MODE_SEND;

    DWORD dwSendBytes;// = send(sock, msgSend, packet.ph.len, 0);
    int iRet = WSASend(m_Sock, &m_wsaSendBuffer, 1, &dwSendBytes, 0, &m_SendOV, NULL);

    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            m_bDisConnect = true;
            return -1;
        }
    }
    return 1;
}

int   TSessionUser::RecvMsg()
{
    m_wsaRecvBuffer.buf = m_szRecvBuffer;
    m_wsaRecvBuffer.len = sizeof(char)*255;
    ZeroMemory(&m_RecvOV, sizeof(OVERLAPPED2));
    m_RecvOV.iType = OVERLAPPED2::MODE_RECV;
    DWORD dwRecvBytes;
    DWORD dwFlag = 0;
    int iRet = WSARecv(m_Sock, &m_wsaRecvBuffer, 1, &dwRecvBytes, &dwFlag, &m_RecvOV, NULL);

    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            m_bDisConnect = true;
            return -1;
        }
    }
    return 1;
}

void    TSessionUser::DispatchRead(DWORD dwTrans)
{
    if (m_iWritePos + dwTrans >= g_iMaxDataBufferSize)
    {
        if (m_iReadPos > 0)
        {
            memmove(m_szDataBuffer, &m_szDataBuffer[m_iPacketPos], m_iReadPos);
        }
        m_iPacketPos = 0;
        m_iWritePos = m_iReadPos;
    }
    // 받은 데이터 누적
    memcpy(&m_szDataBuffer[m_iWritePos], m_szRecvBuffer, dwTrans);
    m_iWritePos += dwTrans;// 다음에 저장되는 버퍼 주소
    m_iReadPos += dwTrans; // 받은 데이터에서 읽은 버퍼 주소

    if (m_iReadPos >= PACKET_HEADER_SIZE)
    {
        // m_iPacketPos = 패킷이 시작되는 주소
        UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
        // 1개의 패킷 이상의 데이터가 존재한다.
        if (packet->ph.len <= m_iReadPos)
        {
            do {
                UPACKET tPacket;
                //tPacket.pUser = this;
                memcpy(&tPacket,
                    &m_szDataBuffer[m_iPacketPos],
                    packet->ph.len);
                // 페킷 풀에 완성 패킷을 넣어주어야 한다.
                m_RecvPacketList.push_back(tPacket);

                m_iReadPos -= packet->ph.len;
                m_iPacketPos += packet->ph.len;

                if (m_iReadPos < PACKET_HEADER_SIZE)
                {
                    break;
                }
                UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
            } while (packet->ph.len <= m_iReadPos);
        }
    }
}
void    TSessionUser::DispatchSend(DWORD dwTrans)
{
}
