// Socket_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "TIocp.h"
#include "TSessionMgr.h"

TSessionMgr    userlist;

// 시작함수
DWORD WINAPI ServerThread(LPVOID lpThreadParameter)
{   
    TSessionMgr* pMgr = (TSessionMgr*)lpThreadParameter;
    while (1)
    {
        Sleep(1);
        for (auto userRecv = pMgr->m_SessionList.begin();
                pMgr->m_SessionList.end() != userRecv; userRecv++)
        {           
            TSessionUser* pUser = *userRecv;
            for (auto packet = pUser->m_RecvPacketList.begin();
                pUser->m_RecvPacketList.end() != packet;
                packet++)
            {     
                switch (packet->ph.type)
                {
                case PACKET_CHAR_MSG:
                {
                  /*  printf("[%s]%s\n", user->m_szName,
                        packet.msg);
                    packet.ph.len += strlen(user->m_szName) + 2;
                    std::string pMsg = "[";
                    pMsg += user->m_szName;
                    pMsg += "]";
                    pMsg += packet.msg;
                    ZeroMemory(packet.msg, 2048);
                    memcpy(packet.msg, pMsg.c_str(), pMsg.size());*/
                }break;

                case PACKET_NAME_REQ:
                {
                   /* memcpy(user->m_szName,
                        packet.msg, strlen(packet.msg));
                    packet.ph.type = PACKET_JOIN_USER;
                    SendMsg(user->sock, nullptr, PACKET_NAME_ACK);*/
                }break;
                }
                for (auto userSend = pMgr->m_SessionList.begin();
                    pMgr->m_SessionList.end() != userSend; userSend++)
                {
                    if (packet->ph.type == PACKET_JOIN_USER)
                    {
                        if (userRecv == userSend)
                        {                           
                            continue;
                        }
                    }
                    (*userSend)->SendMsg(*packet);
                }
            }  
            pUser->m_RecvPacketList.clear();
        }
    }
};
int main()
{
    TIocp  m_Iocp;
    m_Iocp.Init();

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 0;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
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
    HANDLE hClient = CreateThread(0, 0, ServerThread,
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

        
        TSessionUser* pUser = userlist.Add(clientSock, clientaddr);
        m_Iocp.SetBind(clientSock, (ULONG_PTR)pUser);
        pUser->SendMsg(PACKET_CHATNAME_REQ);
    }
    closesocket(listenSock);

    m_Iocp.Release();

    WSACleanup();
    std::cout << "Hello World!\n";
}

