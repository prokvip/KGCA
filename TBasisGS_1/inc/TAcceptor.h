#pragma once
#include "TThread.h"
class TAcceptor : public TThread
{
public:
	SOCKET		m_ListenSock;
public:
	bool		Run();
	bool		Set( int iPort, const char* strAddress = 0 );
	int			GetIPList(vector<string>& ipList);
public:
	TAcceptor(void);
	virtual ~TAcceptor(void);
};
