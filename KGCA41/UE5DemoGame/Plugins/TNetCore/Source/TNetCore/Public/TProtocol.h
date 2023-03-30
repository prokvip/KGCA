#pragma once
#include "PreWindowsApi.h" // winnt.h(536): warning C4005: 'TEXT': macro redefinition
#include "Windows/MinWindows.h"
#include <iostream>
#include <iomanip>//std::setprecision
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

template<typename T>
std::string tostr(const T& value)
{
	std::ostringstream tmp_str;
	tmp_str << value;
	return tmp_str.str();
}

template<typename T, typename ... Args >
std::string tostr(const T& value, const Args& ... args)
{
	return to_str(value) + to_str(args...);
}

struct tGUID : public GUID
{
	friend std::ostream& operator<<(std::ostream& out, const tGUID& h)
	{
		out.write(reinterpret_cast<const char*>(&h), sizeof(tGUID));
		return out;
	}

	friend std::istream& operator>>(std::istream& in, tGUID& h)
	{
		in.read(reinterpret_cast<char*>(&h), sizeof(tGUID));
		return in;
	}
	//friend string ToFGuid(const tGUID& tGuid)
	//{
	//	std::ostringstream tmp_str;
	//	tmp_str << tGuid.Data1 << tGuid.Data3 << tGuid.Data2
	//		<< tGuid.Data4[5] << tGuid.Data4[4] << tGuid.Data4[7] << tGuid.Data4[6]
	//		<< tGuid.Data4[1] << tGuid.Data4[0] << tGuid.Data4[3] << tGuid.Data4[2];
	//	return tmp_str.str();
	//}
	//string ToFGuid()
	//{
	//	std::ostringstream tmp_str;
	//	tmp_str << Data1 << Data3 << Data2
	//		<< Data4[5] << Data4[4] << Data4[7] << Data4[6]
	//		<< Data4[1] << Data4[0] << Data4[3] << Data4[2];
	//	return tmp_str.str();
	//}
	/*friend ostream& operator<<(ostream &stream, const tGUID& tGuid)
	{
		stream << tGuid.Data1 << "";
		stream << tGuid.Data2 << "";
		stream << tGuid.Data3 << "";
		stream << tGuid.Data4[0] << "";
		stream << tGuid.Data4[1] << "";
		stream << tGuid.Data4[2] << "";
		stream << tGuid.Data4[3] << "";
		stream << tGuid.Data4[4] << "";
		stream << tGuid.Data4[5] << "";
		stream << tGuid.Data4[6] << "";
		stream << tGuid.Data4[7] << "";
		return stream;
	}

	friend istream& operator>>(istream& stream, tGUID& tGuid)
	{
		stream >> DWORD(tGuid.Data1);
		stream >> USHORT(tGuid.Data2);
		stream >> USHORT(tGuid.Data3);
		stream >> UCHAR(tGuid.Data4[0]);
		stream >> UCHAR(tGuid.Data4[1]);
		stream >> UCHAR(tGuid.Data4[2]);
		stream >> UCHAR(tGuid.Data4[3]);
		stream >> UCHAR(tGuid.Data4[4]);
		stream >> UCHAR(tGuid.Data4[5]);
		stream >> UCHAR(tGuid.Data4[6]);
		stream >> UCHAR(tGuid.Data4[7]);
		return stream;
	}*/
};

struct GUIDComparer
{
	bool operator()(const tGUID & Left, const tGUID & Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};
struct TVector
{
	float X, Y, Z;
};
struct TRotator
{
	float X, Y, Z;
};
#pragma  pack(push, 1 )

typedef struct {
	uint16_t        iotype;	// 패킷 입출력 종류
	uint16_t    	len;	// msg 바이트 크기
	uint16_t    	type;	// 어떤 패킷이다.	
	uint32_t        time;   // 패킷 전송 시간 
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;
#define PACKET_HEADER_SIZE 10

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8방향
}TPACKET_USER_POSITION;
typedef struct {
	int m_iID;
	int m_iType;
	char m_name[20];
}TPACKET_CREATE_CHARACTER_INFO;

typedef struct {
	tGUID  tGuid;
	FVector vPosition;
	FVector vVelocity;
	FVector vTarget;
	FRotator vRotation;
	float	fHealth;
	int     iType;
	bool	bAlive;
	bool	bAttack;
	bool	bDamage;	
}TPACKET_CHARACTER_INFO;

typedef struct
{
	int     iType;
	FVector vPosition;
}TPACKET_TARGETPOINT_INFO;

#pragma pack(pop)

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000, // 대화명 입력요청	
	PACKET_CHAT_NAME_ACK, // 대화명 입력 응답
	PACKET_CHAT_MSG, // 보통 채팅 메세지.
	PACKET_LOGIN_REQ, // 로그인요청
	PACKET_LOGOUT_REQ, // 로그아웃요청
	PACKET_LOGIN_ACK, // 로그인응답
	PACKET_LOGOUT_ACK, // 로그아웃응답
	PACKET_LOGOUT_PLAYER, // 로그아웃 플레이어

	PACKET_CREATE_ACCOUNT_REQ = 2000, // 계정생성요청
	PACKET_DELETE_ACCOUNT_REQ, // 계정삭제요청
	PACKET_CREATE_ACCOUNT_ACK, // 계정생성응답
	PACKET_DELETE_ACCOUNT_ACK, // 계정삭제응답

	PACKET_USER_POSITION = 3000, // 케릭터 위치 이동 시작
	PACKET_STOP_POSITION,// 케릭터 정지
	PACKET_LOBBY_USER_INFO, // 대기방 유저리스트 정보


	PACKET_CREATE_CHARACTER_REQ = 4000, // 케릭터 생성 요청
	PACKET_DELETE_CHARACTER_REQ, // 케릭터 삭제 요청
	PACKET_CREATE_CHARACTER_ACK, // 케릭터 생성 응답
	PACKET_DELETE_CHARACTER_ACK, // 케릭터 삭제 응답

	PACKET_HIT_CHARACTER = 5000, // 케릭터 피격 받음
	PACKET_HIT_NPC, // NPC 피격 받음
	PACKET_ATTACK_CHARACTER, // 케릭터 공격 시작
	PACKET_ATTACK_NPC, // NPC 공격 시작
	PACKET_DEAD_CHARACTER, // 케릭터 죽음
	PACKET_DEAD_NPC, // NPC 죽음
	PACKET_DAMAGE_CHARACTER, // 케릭터 데미지 받음
	PACKET_DAMAGE_NPC, // NPC 데미지 받음
	PACKET_SPAWN_CHARACTER, // 케릭터 스폰
	PACKET_SPAWN_NPC, // NPC 스폰
	PACKET_SPAWN_NPC_LIST, // 
	PACKET_SYNC_CHARACTER, // 케릭터 동기화
	PACKET_SYNC_NPC_LIST, // NPC 동기화	
	PACKET_NPC_TARGETMOVE,  // NPC 타켓이동	


	PACKET_ZONE_ENTRY_REQ = 6000,//게임존 입장 요청
	PACKET_ZONE_ENTRY_ACK, // 게임 존 입장 응답
	PACKET_ZONE_NEW_PLAYER, // 게임 존 입장 케릭터 정보 전송
	PACKET_ZONE_PLAYERS_INFO, // 게임 존에 입장한 유저들의 플레이 케릭터 정보 전송 
	PACKET_ZONE_USER_INFO,// 게임존 유저리스트 정보(플레이어+기타(옵저버 등))
	PACKET_ZONE_TARGET_REQ, // 존 타켓 위치 요청  
	PACKET_ZONE_TARGET_ACK, // 존 타켓 위치 응답
};


static std::ostream& operator<<(std::ostream &stream, const TPACKET_CHARACTER_INFO& tChar)
{
	stream.write(reinterpret_cast<const char*>(&tChar), sizeof(TPACKET_CHARACTER_INFO));
	return stream;
}

static std::istream& operator>>(std::istream& stream, TPACKET_CHARACTER_INFO& retChar)
{
	TPACKET_CHARACTER_INFO tChar;
	stream.read(reinterpret_cast<char*>(&tChar), sizeof(TPACKET_CHARACTER_INFO));
	retChar = std::move(tChar);
	return stream;
}
template<typename T>
std::istream & tBlockRead(std::istream& stream, T& value) 
{
	return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
template<typename T>
std::ostream & tBlockWrite(std::ostream& stream, T& value)
{
	return stream.write(reinterpret_cast<char*>(&value), sizeof(T));
}

static std::ostream& operator<<(std::ostream& out, const PACKET_HEADER& h)
{
	out.write(reinterpret_cast<const char*>(&h), sizeof(PACKET_HEADER));
	return out;
}

static std::istream& operator>>(std::istream& in, PACKET_HEADER& h)
{
	PACKET_HEADER values;
	in.read(reinterpret_cast<char*>(&values), sizeof(PACKET_HEADER));
	h = std::move(values);
	return in;
}
static std::ostream& operator<<(std::ostream& out, const UPACKET& h)
{
	out.write(reinterpret_cast<const char*>(&h.ph), sizeof(PACKET_HEADER));
	out.write(h.msg, h.ph.len - PACKET_HEADER_SIZE);
	return out;
}

static std::istream& operator>>(std::istream& in, UPACKET& h)
{
	UPACKET uPacket;
	in.read(reinterpret_cast<char*>(&uPacket.ph), sizeof(PACKET_HEADER));
	in.read(uPacket.msg, uPacket.ph.len - PACKET_HEADER_SIZE);
	h = std::move(uPacket);
	return in;
}
static uint32_t g_iPacketID = 0;
static std::stringstream MakePacket(int iType, stringstream& data)
{
	stringstream SendStream;
	PACKET_HEADER header;
	header.iotype = 777;
	header.type = iType;
	header.time = g_iPacketID++;
	header.len = PACKET_HEADER_SIZE + data.str().length();
	SendStream << header;
	SendStream << data.str();
	return std::move(SendStream);
}


static std::ostream& operator<<(std::ostream &stream, const FVector& vector)
{
	stream.write(reinterpret_cast<const char*>(&vector), sizeof(FVector));
	return stream;
}
static std::ostream& operator<<(std::ostream &stream, const FRotator& rot)
{
	stream.write(reinterpret_cast<const char*>(&rot), sizeof(FRotator));
	return stream;
}
static std::istream& operator>>(std::istream &stream,  FVector& vector)
{
	stream.read(reinterpret_cast<char*>(&vector), sizeof(FVector));
	return stream;
}
static std::istream& operator>>(std::istream &stream, FRotator& rot)
{
	stream.read(reinterpret_cast<char*>(&rot), sizeof(FRotator));
	return stream;
}
