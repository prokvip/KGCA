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
void   TSessionMgr::SendPrecess()
{
    for (auto& packet : m_BroadcasttingPacketList)
    {
        for (auto iterSend = m_SessionList.begin(); 
             m_SessionList.end() != iterSend; iterSend++)
        {
            TSessionUser* pUser = (*iterSend);
            pUser->SendMsg(packet);
        }
    }
    m_BroadcasttingPacketList.clear();
}