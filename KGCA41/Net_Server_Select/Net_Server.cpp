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
    SOCKET listensock = socket(AF_INET, SOCK_STREAM, 0);
   

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    //error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(port);

    iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;

    iRet = listen(listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;

   // u_long on = TRUE;
    //ioctlsocket(listensock, FIONBIO, &on);

    SOCKADDR_IN clientaddr;
    SOCKET clientsock;

    std::clock_t start = std::clock();
    printf("서버 정상 작동 시작!!\n");

    /// <summary>
    /// select : 매프레임 마다 초기화->세팅->사용(SELECT)
    /// </summary>
    /// <returns></returns>
    FD_SET readSet;
    FD_SET sendSet;
    FD_SET execptSet;
    timeval timeout;
    timeout.tv_sec = 1.0f;
    timeout.tv_usec = 0;

    while (1) 
    {    
        FD_ZERO(&readSet);
        FD_ZERO(&sendSet);
        FD_ZERO(&execptSet);
        FD_SET(listensock, &readSet);

        for( std::list<SOCKET>::iterator iter = g_userlist.begin();
              iter != g_userlist.end();
              iter++)
        {
            FD_SET(*iter, &readSet);
            FD_SET(*iter, &sendSet);
        }
        iRet = select(0, &readSet, &sendSet, NULL, &timeout);
        if (iRet == SOCKET_ERROR) break;
        if (iRet == 0)
        {
            continue;
        }
        //accept
        if (FD_ISSET(listensock, &readSet))
        {
            int addlen = sizeof(clientaddr);
            clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
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
        }

        //recv or Send
        for (std::list<SOCKET>::iterator iter = g_userlist.begin();
            iter != g_userlist.end();
            )
        {
            if (FD_ISSET(*iter, &readSet))
            {
                SOCKET cSock = *iter;
                char buf[256] = { 0, };
                int iRecvByte = recv(cSock, buf, 256, 0);
                if (iRecvByte == SOCKET_ERROR)
                {
                    closesocket(*iter);
                    iter = g_userlist.erase(iter); 
                    continue;
                }
                else
                {
                    if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
                    {
                        closesocket(*iter);
                        iter = g_userlist.erase(iter);
                        continue;
                    }

                    printf("[받음]%s\n", buf);
                    int iLen = strlen(buf);

                    for (std::list<SOCKET>::iterator iterSend = g_userlist.begin();
                            iterSend != g_userlist.end();
                            )
                    {
                        SOCKET cSock = *iterSend;
                        
                        int iSendByte = send(cSock, buf, iLen, 0);
                        if (iSendByte == SOCKET_ERROR)
                        {                            
                            closesocket(*iterSend);
                            iterSend = g_userlist.erase(iterSend);
                            continue;
                        }
                        iterSend++;
                    }
                }
            }
            /*for (std::list<SOCKET>::iterator iter = g_userlist.begin();
                iter != g_userlist.end();
                iter++)
            {
                if (FD_ISSET(*iter, &sendSet))
                {
                    break;
                }
            }*/
            iter++;
        }
        

        //        for (std::list<SOCKET>::iterator iterSend = g_userlist.begin();
        //            iterSend != g_userlist.end();
        //            iterSend++)
        //        {
        //            SOCKET cSock = *iterSend;
        //                
        //            int iSendByte = send(cSock, buf, iLen, 0);
        //            if (iSendByte == SOCKET_ERROR)
        //            {
        //                int iError = WSAGetLastError();
        //                if (iError != WSAEWOULDBLOCK)
        //                {
        //                    break;
        //                }
        //            }
        //        }
        //            
        //    }
        //    iter++;     
        //       /* double time = (std::clock() - start) / (double)CLOCKS_PER_SEC;                
        //        if (time > g_time + 5)
        //        {
        //            g_time = (int)time;

        //            char sendbuf[256] = "kgca";
        //            int iLen = strlen(sendbuf);
        //            int iSendByte = send(clientsock, sendbuf, iLen, 0);
        //            if (iSendByte == SOCKET_ERROR)
        //            {
        //                int iError = WSAGetLastError();
        //                if (iError != WSAEWOULDBLOCK)
        //                {
        //                    break;
        //                }
        //            }
        //            printf("%d바이트를 전송했습니다\n", iSendByte);
        //        }*/
        //    
        //    /*closesocket(clientsock);
        //    printf("클라이언트 접속해제 ip=%s, Port:%d\n",
        //        inet_ntoa(clientaddr.sin_addr),
        //        ntohs(clientaddr.sin_port));*/
        //}        
    }
    closesocket(listensock);
    // )  윈속 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}
