#include "TNetwork.h"
void    TNetwork::PacketProcess()
{
    FD_ZERO(&m_rSet);
    FD_ZERO(&m_wSet);
    FD_SET(m_Sock, &m_rSet);
    FD_SET(m_Sock, &m_wSet);
    for (auto& user : userlist)
    {
        FD_SET(user.sock, &m_rSet);
        FD_SET(user.sock, &m_wSet);
    }
    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 1000; // 1/1000000초
    int iRet = select(0, &m_rSet, &m_wSet, NULL, &time);
    if (iRet == SOCKET_ERROR) { return; }
    if (iRet == 0) { return; } // timeout

    if (FD_ISSET(m_Sock, &m_rSet))
    {
        // 접속되면 반환된다.
        SOCKADDR_IN clientaddr;
        int length = sizeof(clientaddr);
        SOCKET clientSock = accept(m_Sock, (sockaddr*)&clientaddr, &length);
        if (clientSock == SOCKET_ERROR)
        {
            closesocket(m_Sock);
            WSACleanup();
            return;
        }
        printf("클라이언트 접속 : IP:%s, PORT:%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        u_long iMode = TRUE;
        ioctlsocket(clientSock, FIONBIO, &iMode);

        TUser user;
        user.sock = clientSock;
        user.address = clientaddr;
        userlist.push_back(user);

        SendMsg(clientSock, nullptr, 0, PACKET_CHATNAME_REQ);
    }
    for (auto iterRecv = userlist.begin();
        userlist.end() != iterRecv; )
    {
        UPACKET packet;
        ZeroMemory(&packet, sizeof(UPACKET));

        if (FD_ISSET(iterRecv->sock, &m_rSet))
        {
            int iRecvPacketSize = PACKET_HEADER_SIZE;

            int iRecvBytes = recv(iterRecv->sock, iterRecv->szRecvMsg,
                PACKET_HEADER_SIZE - iterRecv->iTotalRecvBytes, 0);
            if (iRecvBytes == 0)
            {
                printf("클라이언트 접속 종료 : IP:%s, PORT:%d\n",
                    inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                closesocket(iterRecv->sock);
                iterRecv = userlist.erase(iterRecv);
                continue;
            }
            DWORD dwError = WSAGetLastError();
            if (iRecvBytes == SOCKET_ERROR)
            {
                if (dwError != WSAEWOULDBLOCK)
                {
                    //WSAEWOULDBLOCK 아니라면 오류!
                    closesocket(iterRecv->sock);
                    iterRecv = userlist.erase(iterRecv);
                }
                else
                {
                    iterRecv++;
                }
                continue;
            }

           

            iterRecv->iTotalRecvBytes += iRecvBytes;
            if (iterRecv->iTotalRecvBytes == PACKET_HEADER_SIZE)
            {
                memcpy(&packet.ph, iterRecv->szRecvMsg, PACKET_HEADER_SIZE);

                char* msg = (char*)&packet;
                int iNumRecvByte = 0;
                do {
                    int iRecvBytes = recv(iterRecv->sock,
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
                            closesocket(iterRecv->sock);
                            printf("서버 비정상 종료\n");
                            return;
                        }
                        continue;
                    }
                    iNumRecvByte += iRecvBytes;
                } while ((packet.ph.len - PACKET_HEADER_SIZE) > iNumRecvByte);
            }

            if (iRecvBytes > 0)
            {
                m_RecvPacketList.push_back(packet);
            }           
            
            //ZeroMemory(&packet, sizeof(UPACKET));
            iterRecv->iTotalRecvBytes = 0;
        }

        if(FD_ISSET(iterRecv->sock, &m_wSet))
        {
            if (packet.ph.type == PACKET_NAME_REQ)
            {
                SendMsg(iterRecv->sock, nullptr, 0, PACKET_NAME_ACK);
            }
        }
        iterRecv++;
    }


    for (auto& packet : m_RecvPacketList)
    {
        TNetwork::FunIter iter = m_fnExecutePacket.find(packet.ph.type);
        if (iter != m_fnExecutePacket.end())
        {
            TNetwork::CallFuction call = iter->second;
            call(packet);
        }
    }
    m_RecvPacketList.clear();
}
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
    m_BroadcasttingPacketList.push_back(packet);
}
void   TNetwork::SendPrecess()
{
    //for (auto& packet : m_SendPacketList)
    //{
    //    char* msgSend = (char*)&packet;
    //    int iSendBytes = send(m_Sock, msgSend, packet.ph.len, 0);

    //    if (iSendBytes == SOCKET_ERROR)
    //    {
    //        if (WSAGetLastError() != WSAEWOULDBLOCK)
    //        {
    //            //WSAEWOULDBLOCK 아니라면 오류!
    //            closesocket(m_Sock);
    //            break;
    //        }
    //    }
    //}
    //m_SendPacketList.clear();

    for (auto& packet : m_BroadcasttingPacketList)
    {
        for (auto iterSend = userlist.begin(); userlist.end() != iterSend; )
        {
            int iSendBytes = send(iterSend->sock, (char*)&packet,
                packet.ph.len, 0);

            if (iSendBytes == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                {
                    //WSAEWOULDBLOCK 아니라면 오류!
                    printf("클라이언트 접속 비정상 종료 : IP:%s, PORT:%d\n",
                        inet_ntoa(iterSend->address.sin_addr), ntohs(iterSend->address.sin_port));
                    closesocket(iterSend->sock);
                    iterSend = userlist.erase(iterSend);
                    continue;
                }
            }
            iterSend++;
        }
    }
    m_BroadcasttingPacketList.clear();
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
    SOCKADDR_IN sa;//목적지+포트
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(iPort);
    int iRet = bind(m_Sock, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR)
    {
        return 1;
    }
    iRet = listen(m_Sock, SOMAXCONN);
    if (iRet == SOCKET_ERROR)
    {
        return 1;
    }
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