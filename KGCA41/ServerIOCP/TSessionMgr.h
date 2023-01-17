#pragma once
#include "TSessionUser.h"
class TSessionMgr
{
public:
	std::list<TSessionUser*> m_SessionList;
	TSessionUser* Add(SOCKET sock, SOCKADDR_IN address);
};

