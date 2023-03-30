#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TLock.h"

void TServer::SendLogoutPlayer(tGUID tGuid)
{
	stringstream data;
	data << tGuid;
	SendBroadcastPool(MakePacket(PACKET_LOGOUT_PLAYER, data));
	//Broadcast(tGuid, MakePacket(PACKET_LOGOUT_PLAYER, data));
}
void TServer::ReqVerification(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);

	tGUID guid;
	RecvStream >> guid;

	int iRet = -1;
	if (pSendUser.pUser->m_LoginGuid == guid)
	{
		iRet = 1;
	}
	stringstream data;
	// ret : 1(OK),2(ID-Faild),3(PS-Faild),4(etc)
	data << iRet;
	if (iRet == 1)
	{
		data << pSendUser.pUser->m_tGuid;
		data << pSendUser.pUser->m_GuidActiveCharacter;
	}
	else
	{
		tGUID nullguid;
		data << nullguid;
		data << nullguid;

		string guidString;
		RPC_CSTR szUuid = NULL;
		if (::UuidToStringA(&guid, &szUuid) == RPC_S_OK)
		{
			guidString = (char*)szUuid;
			::RpcStringFreeA(&szUuid);
		}
		//T_LOG("%s=%s\r\n", guidString.c_str(), "ReqVerification Error!");
	}
	SendPacket(pSendUser.pUser, MakePacket(PACKET_VERIFICATION_ACK, data));
	T_LOG("%s\r\n", "PACKET_VERIFICATION_ACK");
}
void TServer::CreateAccount(T_PACKET& pSendUser)
{
	tGUID uuid;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	string Id;
	string Pw;
	RecvStream >> Id;
	RecvStream >> Pw;
	stringstream data;
	// ret : 1(OK),2(ID-Faild),3(PS-Faild),4(etc)
	data << 1;
	data << uuid;
	SendPacket(pSendUser.pUser, MakePacket(PACKET_CREATE_ACCOUNT_ACK, data));
}
void TServer::DeleteAccount(T_PACKET& pSendUser)
{
}
void TServer::Login(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);

	string Id;
	string Pw;
	RecvStream >> Id;
	RecvStream >> Pw;
	stringstream data;
	// ret : 1(OK),2(ID-Faild),3(PS-Faild),4(etc)
	int iRet = 1;
	data << iRet;
	data << pSendUser.pUser->m_LoginGuid;
	data << pSendUser.pUser->m_tGuid;
	data << pSendUser.pUser->m_GuidActiveCharacter;
	SendPacket(pSendUser.pUser, MakePacket(PACKET_LOGIN_ACK, data));

	PrintGuid(pSendUser.pUser->m_LoginGuid, "loginguid");
	PrintGuid(pSendUser.pUser->m_tGuid, "guid");
	PrintGuid(pSendUser.pUser->m_GuidActiveCharacter, "character");
}
void TServer::Logout(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	tGUID tGuid;
	RecvStream >> tGuid;
	SendLogoutPlayer(tGuid);
}