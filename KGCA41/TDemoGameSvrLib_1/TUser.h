#pragma once
#include "TPlayerCharacter.h"
#include "TCircularBuf.h"
#define MAX_USER_RECV_SIZE 512

class TSession : public TServerObj
{
public:
	SOCKET			m_Socket;
	SOCKET			m_SendSocket;
	SOCKADDR_IN		m_addr;
public:
	TSession();
	virtual ~TSession();
};
class TBaseUser : public TSession
{
public:
	virtual void  Create() = 0;
	virtual void  Destory() = 0;
	virtual void  SetSocket(SOCKET sock, SOCKADDR_IN  client) = 0;
public:
	TBaseUser() {}
	virtual ~TBaseUser() {}
};

class TUser : public TBaseUser
{
public:
	RUNTIME_DECLARE(TUser)
private:	
	TCharacter*     m_pCharacter;
	TCharacterList  m_CharacterList;
	TServerObj		m_LockOverlapped;
	TStreamPacket	m_StreamPacket;
	CircularBuffer  mRecvBuffer;
public:
	void			OnRead(size_t len);
	TCharacterList& GetCharacterList();
	void			SetActiveCharacter(TCharacter*, bool bAlive);
public:
	OVERLAPPED2		m_Overlapped;
	OVERLAPPED2		m_OverlappedZero;
	OVERLAPPED2*    Overlapped(int iType);
	OVERLAPPED2*    OverlappedZero();
	int				m_iType;
	tGUID		    m_LoginGuid;
	tGUID		    m_tGuid;
	tGUID			m_GuidActiveCharacter;
	CONNECT_TYPE    m_ConnectType;
	string			m_Name;
	WSABUF			m_wsaRecvBuffer;
	WSABUF			m_wsaZeroBuffer;
	WSABUF			m_wsaSendBuffer;
	char			m_strBuffer[MAX_USER_RECV_SIZE];
	char			m_sendBuffer[MAX_USER_RECV_SIZE];	
	bool			m_bUsed;
public:
	bool	    Dispatch(DWORD deByteSize, LPOVERLAPPED ov);
	bool		WaitForPacketReceive();
	bool		WaitForZeroByteReceive();
	bool		WaitReceive();
	void	Release();
	void	Disconnect();
public:
	virtual void  Create() override {}
	virtual void  Destory() override {};
	virtual void SetSocket(SOCKET sock, SOCKADDR_IN addr) override
	{
		//WSAPROTOCOL_INFO ProtocolInfo;
		//DWORD dwProcID = GetCurrentProcessId();
		//int iRet = WSADuplicateSocket(sock, dwProcID, &ProtocolInfo);	//	DuplicateHandle()
		//m_SendSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, &ProtocolInfo, 0, 0);
		m_Socket = sock;
		m_addr = addr;
	};
public:
	friend ostream& operator<<(ostream &stream, TUser& info);
	friend istream& operator>>(istream& stream, TUser& info);
public:
	TUser();
	virtual ~TUser();
};
