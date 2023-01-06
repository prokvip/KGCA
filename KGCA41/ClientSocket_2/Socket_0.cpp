// Socket_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include "TProtocol.h"

int   SendMsg(SOCKET sock, char* msg, short type)
{
    UPACKET packet;
    ZeroMemory(&packet, sizeof(UPACKET));
    packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
    packet.ph.type = type;
    memcpy(packet.msg,msg, strlen(msg));

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
// 시작함수
DWORD WINAPI SendThread(LPVOID lpThreadParameter)
{
    SOCKET sock = (SOCKET)lpThreadParameter;
    while (1)
    {
        char szSendMsg[256] = { 0, };
        printf("%s ", "->");
        fgets(szSendMsg, 256, stdin);
        szSendMsg[strlen(szSendMsg)-1] = 0;
        if (strcmp(szSendMsg, "exit")==0)
        {
            break;
        }
        if (SendMsg(sock, szSendMsg, PACKET_CHAR_MSG)<0)
        {
            break;
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
                                    (LPVOID)sock3, CREATE_SUSPENDED, &dwThreadID);
    

    //u_long iMode = TRUE;
    //ioctlsocket(sock3, FIONBIO, &iMode);   
    //char szSendMsg[256] = "kgca";// { 0, };
    //printf("%s", "send---->");

    int iRecvPacketSize = PACKET_HEADER_SIZE;
    int iTotalRecvBytes = 0;
    while (1)
    {       
        Sleep(1);
        char szRecvMsg[256] = { 0, };         
        int iRecvBytes = recv(sock3, szRecvMsg, 
            PACKET_HEADER_SIZE - iTotalRecvBytes, 0);
        if (iRecvBytes == 0)
        {
            printf("서버 정상 종료\n");
            break;
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
                int iRecvBytes = recv(sock3, 
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
                        closesocket(sock3);
                        printf("서버 비정상 종료\n");
                        return 1;
                    }
                    continue;
                }
                iNumRecvByte += iRecvBytes;

               
            } while ((packet.ph.len- PACKET_HEADER_SIZE) > iNumRecvByte);
            
            switch (packet.ph.type)
            {
                case PACKET_CHAR_MSG:
                {
                    printf("%s\n", packet.msg);
                }break;

                case PACKET_CHATNAME_REQ:
                {
                    printf("이름을 입력하시오 : ");
                    char szName[256] = { 0, };
                    fgets(szName, 256, stdin);
                    szName[strlen(szName) - 1] = 0;
                    SendMsg(sock3, szName, PACKET_NAME_REQ);
                    ResumeThread(hClient);
                }break;

                case PACKET_JOIN_USER:
                {
                    printf("%s %s\n", packet.msg,"님이 입장하였습니다.");
                }break;
                case PACKET_NAME_ACK:
                {
                    printf("대화명 사용 승인\n");
                }break;
            }   

            iTotalRecvBytes = 0;
        }            
    }

    CloseHandle(hClient);
    closesocket(sock3);
    WSACleanup();
    std::cout << "Hello World!\n";

    Sleep(5000);
}