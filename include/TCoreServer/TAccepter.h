#pragma once
#include "TThread.h"
class TAccepter :   public TThread
{
	SOCKET				 m_ListenSock;
public:
	virtual bool	Set(int iPort);
	bool			Run() override;
public:
	TAccepter();
	TAccepter(LPVOID value);
	virtual ~TAccepter();
};

