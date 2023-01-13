#include "Sample.h"
void   Sample::ChatMsg(UPACKET& t)
{
    std::wstring fmt = L"[%s]";
    Print(fmt.c_str(), to_mw(t.msg).c_str());
    
    //UPACKET packet;
    //m_Net.MakePacket(packet, t.msg, strlen(t.msg), PACKET_CHAR_MSG); 
    m_Net.m_BroadcasttingPacketList.push_back(t);
}
void   Sample::NameReq(UPACKET& t)
{
   /* UPACKET packet1;
    m_Net.MakePacket(packet1, nullptr, 0, PACKET_NAME_ACK);
    m_Net.m_SendPacketList.insert(std::make_pair(sock, packet1));*/

    UPACKET packet2;
    m_Net.MakePacket(packet2, nullptr, 0, PACKET_JOIN_USER);
    m_Net.m_BroadcasttingPacketList.push_back(packet2);
}