#include "TSessionMgr.h"
TSessionUser* TSessionMgr::Add(SOCKET sock, SOCKADDR_IN address)
{
	//u_long iMode = TRUE;
	//ioctlsocket(sock, FIONBIO, &iMode);

	TSessionUser* user = new TSessionUser;
	user->Set(sock, address);	
	m_SessionList.push_back(user);
	return user;
}