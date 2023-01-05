// Socket_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <list>
#include <winsock2.h>

struct TUser
{
    SOCKET sock;
    SOCKADDR_IN address;
};
std::list<TUser> userlist;

// 시작함수
DWORD WINAPI ServerThread(LPVOID lpThreadParameter)
{
    TUser* user = (TUser*)lpThreadParameter;
    SOCKET sock = (SOCKET)user->sock;
    while (1)
    {
        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(sock, szRecvMsg, 256, 0);
        if (iRecvBytes == 0)
        {
            for (auto iter = userlist.begin();
                userlist.end() != iter;
                iter++)
            {
                if (iter->sock == sock)
                {
                    printf("클라이언트 접속 종료 : IP:%s, PORT:%d\n",
                        inet_ntoa(iter->address.sin_addr), ntohs(iter->address.sin_port));
                    closesocket(sock);
                    userlist.erase(iter);
                    break;
                }
            }
           
            
            break;
        }
        if (iRecvBytes == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                //WSAEWOULDBLOCK 아니라면 오류!
                closesocket(sock);
                return 1;
            }
        }
        /*else
        {
            printf("%s\n", szRecvMsg);
        }*/

        if (iRecvBytes > 0)
        {
            for (auto iter = userlist.begin();
                userlist.end() != iter;
                )
            {
                int iSendBytes = send(iter->sock, szRecvMsg, strlen(szRecvMsg), 0);
                if (iSendBytes == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSAEWOULDBLOCK)
                    {
                        //WSAEWOULDBLOCK 아니라면 오류!
                        printf("클라이언트 접속 비정상 종료 : IP:%s, PORT:%d\n",
                            inet_ntoa(iter->address.sin_addr), ntohs(iter->address.sin_port));
                        closesocket(iter->sock);
                        iter = userlist.erase(iter);
                        continue;
                    }
                }
                iter++;
            }
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
        TUser user;
        user.sock = clientSock;
        user.address = clientaddr;
        userlist.push_back(user);

        DWORD dwThreadID;
        HANDLE hClient = CreateThread(  0, 0, ServerThread,
                                        (LPVOID)&user, 0, &dwThreadID);

        //u_long iMode = TRUE;
        //ioctlsocket(clientSock, FIONBIO, &iMode);       
    }
    closesocket(listenSock);

    WSACleanup();
    std::cout << "Hello World!\n";
}

