// Socket_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
// 시작함수
DWORD WINAPI SendThread(LPVOID lpThreadParameter)
{
    SOCKET sock = (SOCKET)lpThreadParameter;
    while (1)
    {
        char szSendMsg[256] = { 0, };
        printf("%s", "send---->");
        fgets(szSendMsg, 256, stdin);
        szSendMsg[strlen(szSendMsg)-1] = 0;
        if (strcmp(szSendMsg, "exit")==0)
        {
            break;
        }
        int iSendBytes = send(sock, szSendMsg, strlen(szSendMsg), 0);
        if (iSendBytes == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                //WSAEWOULDBLOCK 아니라면 오류!
                closesocket(sock);
                return 1;
            }            
        }
    }
    closesocket(sock);
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
    // 블록형 소켓 <-> 넌블록형 소켓
    SOCKET sock3 = socket(AF_INET, SOCK_STREAM, 0);
  
    

    short sData = 10000;
    short tData = 0x2710;
    short fData = 0x1027;

    // client
    SOCKADDR_IN sa;//목적지+포트
    sa.sin_family = AF_INET;
    //error C4996: 'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr("192.168.0.12");
    sa.sin_port = htons(10000);
    int iRet = connect(sock3, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        printf("%d ", iError);
        return 1;
    }    

    //win api
    DWORD dwThreadID;
    HANDLE hClient = CreateThread(  0, 0, SendThread,
                                    (LPVOID)sock3, 0, &dwThreadID);
    

    u_long iMode = TRUE;
    ioctlsocket(sock3, FIONBIO, &iMode);   
    //char szSendMsg[256] = "kgca";// { 0, };
    //printf("%s", "send---->");

    while (1)
    {       
        Sleep(100);
        char szRecvMsg[256] = { 0, }; 
        int iRecvBytes = recv(sock3, szRecvMsg, 256, 0);
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
                closesocket(sock3);
                printf("서버 비정상 종료\n");
                return 1;
            }            
            continue;
        }
        printf("Recv---->%s\n", szRecvMsg);        
    }

    CloseHandle(hClient);
    closesocket(sock3);
    WSACleanup();
    std::cout << "Hello World!\n";

    Sleep(5000);
}