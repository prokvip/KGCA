#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
const short port = 10000;
// (1)socket
int main()
{
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
            //u_long on = FALSE;
            //ioctlsocket(clientsock, FIONBIO, &on);
            printf("클라이언트 접속 ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));
            while (1)
            {
                char buf[256] = { 0, };
                int iRecvByte = recv(clientsock, buf, 256, 0);
                if (iRecvByte == SOCKET_ERROR)
                {
                    int iError = WSAGetLastError();
                    if (iError != WSAEWOULDBLOCK)
                    {
                        break;
                    }
                }
                else
                {
                    if (iRecvByte == 0) break; // 정상종료
                    if (iRecvByte == SOCKET_ERROR) break;// 비정상 종료
                    printf("[받음]%s\n", buf);

                    int iLen = strlen(buf);
                    int iSendByte = send(clientsock, buf, iLen, 0);
                    if (iSendByte == SOCKET_ERROR)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            break;
                        }
                    }
                    printf("%d바이트를 전송했습니다", iSendByte);
                }
            }
            closesocket(clientsock);
            printf("클라이언트 접속해제 ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));
        }        
    }
    closesocket(sock);
    // )  윈속 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}
