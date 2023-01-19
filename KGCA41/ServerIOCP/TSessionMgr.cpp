#include "TSessionMgr.h"
TSessionUser* TSessionMgr::Add(SOCKET sock, SOCKADDR_IN address)
{
	//u_long iMode = TRUE;
	//ioctlsocket(sock, FIONBIO, &iMode);
    auto user = m_Pool.NewChunk();
    user->Set(sock, address);	
	m_SessionList.push_back(user);
	return user.get();
}
void   TSessionMgr::Delete(std::shared_ptr<TSessionUser> user)
{
    m_Pool.DeleteChunk(user);
}
void   TSessionMgr::SendPrecess()
{
    for (auto& packet : m_BroadcasttingPacketList)
    {
        for (auto iterSend = m_SessionList.begin(); 
             m_SessionList.end() != iterSend; iterSend++)
        {
            TSessionUser* pUser = (*iterSend).get();
            pUser->SendMsg(packet);
        }
    }
    m_BroadcasttingPacketList.clear();
}