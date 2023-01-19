#pragma once
#include "TServerStd.h"
#include "TObjectPool.h"

#define g_iMaxRecvBufferSize 256
#define g_iMaxDataBufferSize (PACKET_MAX_DATA_SIZE+PACKET_HEADER_SIZE*2)

struct OVERLAPPED2 : TObjectPool< OVERLAPPED2>
{    
    enum { MODE_RECV = 1, MODE_SEND = 2, MODE_EXIT =3};
    OVERLAPPED ov;
    int  iType;
    OVERLAPPED2()
    {
        memset(&ov, 0, sizeof(OVERLAPPED));
        iType = 0;
    }
    OVERLAPPED2(int type)
    {
        memset(&ov, 0, sizeof(OVERLAPPED));
        iType = type;
    }
};
class TSessionUser
{
public:
    bool        m_bDisConnect=false;
    SOCKET      m_Sock;
    SOCKADDR_IN m_Address;
    char    m_szName[9] = { 0, };
public:
    //char    m_szRecvBuffer[g_iMaxRecvBufferSize] = { 0, };
    // �迭 ����� �Ѱ�->����Ʈ+�迭(���Ѵ�)
    char    m_szDataBuffer[g_iMaxDataBufferSize] = { 0, };
    int		m_iPacketPos=0; // ��Ŷ�� ���� �ּ�
    int		m_iWritePos = 0; // ��Ŷ ������ ���� �ּ�
    int		m_iReadPos = 0; // ��Ŷ ������ ���� �ּ�

    WSABUF      m_wsaRecvBuffer;    
    WSABUF      m_wsaSendBuffer;

    std::list<UPACKET>  m_RecvPacketList;
    std::list<UPACKET>  m_SendPacketList;
public:
    bool    Set(SOCKET sock,SOCKADDR_IN address);
    int     SendMsg(short type, char* msg=nullptr );
    int     SendMsg(UPACKET& packet);
    int     RecvMsg();
    void    DispatchRead(DWORD dwTrans,OVERLAPPED2* pOV2);
    void    DispatchSend(DWORD dwTrans,OVERLAPPED2* pOV2);
public:
    TSessionUser()
    {
        ZeroMemory(m_szName, sizeof(char)*1);
    }
};

