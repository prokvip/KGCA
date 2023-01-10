#include "TNetwork.h"

void  TNetwork::MakePacket(UPACKET& packet, const char* msg, int iSize, short type)
{
    ZeroMemory(&packet, sizeof(UPACKET));
    packet.ph.len = iSize + PACKET_HEADER_SIZE;
    packet.ph.type = type;
    memcpy(packet.msg, msg, iSize);
}
void  TNetwork::RecvPrecess()
{   
    int iRecvPacketSize = PACKET_HEADER_SIZE;
    int iTotalRecvBytes = 0;
    while (1)
    {
        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(m_Sock, szRecvMsg,
            PACKET_HEADER_SIZE - iTotalRecvBytes, 0);
        if (iRecvBytes == 0)
        {
            printf("서버 정상 종료\n");
            return;
        }
        if (iRecvBytes == SOCKET_ERROR)
        {
            DWORD dwError = WSAGetLastError();
            if (dwError != WSAEWOULDBLOCK)
            {
                closesocket(m_Sock);            
            }      
            return;            
        }
        iTotalRecvBytes += iRecvBytes;
        if (iTotalRecvBytes == PACKET_HEADER_SIZE)
        {
            UPACKET packet;
            ZeroMemory(&packet, sizeof(UPACKET));
            memcpy(&packet.ph, szRecvMsg, PACKET_HEADER_SIZE);

            char* msg = (char*)&packet;
            int iNumRecvByte = 0;
            do {
                if (packet.ph.len == 4)
                {
                    break;
                }
                int iRecvBytes = recv(m_Sock,
                    &packet.msg[iNumRecvByte],
                    packet.ph.len - PACKET_HEADER_SIZE - iNumRecvByte, 0);

                if (iRecvBytes == 0)
                {
                    printf("서버 정상 종료\n");
                    break;
                }
                if (iRecvBytes == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSAEWOULDBLOCK)
                    {
                        //WSAEWOULDBLOCK 아니라면 오류!
                        closesocket(m_Sock);
                        printf("서버 비정상 종료\n");
                        return;
                    }
                    continue;
                }
                iNumRecvByte += iRecvBytes;


            } while ((packet.ph.len - PACKET_HEADER_SIZE) > iNumRecvByte);

            m_RecvPacketList.push_back(packet);
            iTotalRecvBytes = 0;
            return;
        }
    }
};
void   TNetwork::AddSend(SOCKET sock, const char* data, int iSize, short type)
{
    UPACKET packet;
    MakePacket(packet, data, iSize, type);
    m_SendPacketList.push_back(packet);
}
void   TNetwork::SendPrecess()
{
    for (auto& packet : m_SendPacketList)
    {
        char* msgSend = (char*)&packet;
        int iSendBytes = send(m_Sock, msgSend, packet.ph.len, 0);

        if (iSendBytes == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                //WSAEWOULDBLOCK 아니라면 오류!
                closesocket(m_Sock);
                break;
            }
        }
    }
    m_SendPacketList.clear();
}
int   TNetwork::SendMsg(SOCKET sock, const char* data, int iSize, short type)
{
    UPACKET packet;
    MakePacket(packet, data, iSize, type);

    char* msgSend = (char*)&packet;
    int iSendBytes = send(sock, msgSend, packet.ph.len, 0);

    if (iSendBytes == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            //WSAEWOULDBLOCK 아니라면 오류!
            closesocket(sock);
            return -1;
        }
    }
    return 1;
}
bool   TNetwork::NetStart(std::string ip, int iPort)
{
    m_Sock = socket(AF_INET, SOCK_STREAM, 0);
    // 셀렉트 모델 적용
    int iRet = WSAAsyncSelect(m_Sock, g_hWnd, NETWORK_MSG, 
                    FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    if (iRet == SOCKET_ERROR)
    {
        return false;
    }

    // client
    SOCKADDR_IN sa;//목적지+포트
    sa.sin_family = AF_INET;
    //error C4996: 'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    sa.sin_port = htons(iPort);
    iRet = connect(m_Sock, (sockaddr*)&sa, sizeof(sa));
   /* if (iRet == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        printf("%d ", iError);
        return false;
    }*/

   // u_long iMode = TRUE;
   // ioctlsocket(m_Sock, FIONBIO, &iMode);

    return true;
}
bool TNetwork::Frame()
{    
    return true;
}
bool TNetwork::Render()
{
    return true;
}
bool TNetwork::Release()
{
    closesocket(m_Sock);
    return true;
}
TNetwork::TNetwork()
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
}

TNetwork::~TNetwork()
{
	WSACleanup();
}