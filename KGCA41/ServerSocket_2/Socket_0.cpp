// Socket_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <list>
#include <winsock2.h>
#include "TProtocol.h"

struct TUser
{
    SOCKET sock;
    SOCKADDR_IN address;
    char szRecvMsg[255] = { 0, };
    int iTotalRecvBytes = 0;
};
std::list<TUser> userlist;

// 시작함수
DWORD WINAPI ServerThread(LPVOID lpThreadParameter)
{    
    while (1)
    {
        for (auto iterRecv = userlist.begin();
            userlist.end() != iterRecv; )
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

            UPACKET packet;
            ZeroMemory(&packet, sizeof(UPACKET));

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
                            return 1;
                        }
                        continue;
                    }
                    iNumRecvByte += iRecvBytes;
                } while ((packet.ph.len - PACKET_HEADER_SIZE) > iNumRecvByte);
            }


            if (iRecvBytes > 0)
            {
                printf("%s\n", packet.msg);
                for (auto iterSend = userlist.begin();
                    userlist.end() != iterSend;
                    )
                {
                    int iSendBytes = send(  iterSend->sock, (char*)&packet, 
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

                ZeroMemory(&packet, sizeof(UPACKET));
                iterRecv->iTotalRecvBytes = 0;
            }
            iterRecv++;
        }
    }
};
int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 0;
    }

    //SOCKET sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //SOCKET sock2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    short sData = 10000;
    short tData = 0x2710;
    short fData = 0x1027;

    // client
    SOCKADDR_IN sa;//목적지+포트
    sa.sin_family = AF_INET;   
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(10000);
    int iRet = bind(listenSock, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR)
    {
        return 1;
    }
    iRet = listen(listenSock, SOMAXCONN);
    if (iRet == SOCKET_ERROR)
    {
        return 1;
    }
   
    DWORD dwThreadID;
    HANDLE hClient = CreateThread(  0, 0, ServerThread,
                                    0, 0, &dwThreadID);

    while (1)
    {
        // 접속되면 반환된다.
        SOCKADDR_IN clientaddr;
        int length = sizeof(clientaddr);
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientaddr, &length);
        if (clientSock == SOCKET_ERROR)
        {
            closesocket(listenSock);
            WSACleanup();
            return 1;
        }
        printf("클라이언트 접속 : IP:%s, PORT:%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        
        u_long iMode = TRUE;
        ioctlsocket(clientSock, FIONBIO, &iMode); 

        TUser user;
        user.sock = clientSock;
        user.address = clientaddr;
        userlist.push_back(user);        

              
    }
    closesocket(listenSock);

    WSACleanup();
    std::cout << "Hello World!\n";
}

