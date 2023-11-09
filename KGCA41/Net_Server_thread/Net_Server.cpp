#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
const short port = 10000;
int g_time = 0;

std::list<SOCKET>  g_userlist;

int main()
{    
    /*while(1)
    {
        double time = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        std::cout << time << std::endl;
    }*/
    // 1) 윈속 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    //                           IP                    TCP
    //SOCKET sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // 넌블록형 소켓 할당
    u_long on = TRUE;
    ioctlsocket(sock, FIONBIO, &on);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    //error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(port);

    iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;

    iRet = listen(sock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;

    SOCKADDR_IN clientaddr;
    SOCKET clientsock;

    std::clock_t start = std::clock();
    printf("서버 정상 작동 시작!!\n");
    while (1) {       
        int addlen = sizeof(clientaddr);
        clientsock = accept(sock, (SOCKADDR*)&clientaddr, &addlen);
        if (clientsock == SOCKET_ERROR)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
        }
        else
        {
            g_userlist.push_back(clientsock);
            printf("클라이언트 접속 ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));
        }
        for( std::list<SOCKET>::iterator iter = g_userlist.begin();
                iter != g_userlist.end();
                )
        {
            SOCKET cSock = *iter;
            char buf[256] = { 0, };
            int iRecvByte = recv(cSock, buf, 256, 0);
            if (iRecvByte == SOCKET_ERROR)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    iter = g_userlist.erase(iter);
                    continue;
                }
            }
            else
            {
                if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
                {
                    iter = g_userlist.erase(iter);
                    continue;
                }
                    
                printf("[받음]%s\n", buf);    
                int iLen = strlen(buf);

                for (std::list<SOCKET>::iterator iterSend = g_userlist.begin();
                    iterSend != g_userlist.end();
                    iterSend++)
                {
                    SOCKET cSock = *iterSend;
                        
                    int iSendByte = send(cSock, buf, iLen, 0);
                    if (iSendByte == SOCKET_ERROR)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            break;
                        }
                    }
                }
                    
            }
            iter++;     
               /* double time = (std::clock() - start) / (double)CLOCKS_PER_SEC;                
                if (time > g_time + 5)
                {
                    g_time = (int)time;

                    char sendbuf[256] = "kgca";
                    int iLen = strlen(sendbuf);
                    int iSendByte = send(clientsock, sendbuf, iLen, 0);
                    if (iSendByte == SOCKET_ERROR)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            break;
                        }
                    }
                    printf("%d바이트를 전송했습니다\n", iSendByte);
                }*/
            
            /*closesocket(clientsock);
            printf("클라이언트 접속해제 ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));*/
        }        
    }
    closesocket(sock);
    // )  윈속 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}
