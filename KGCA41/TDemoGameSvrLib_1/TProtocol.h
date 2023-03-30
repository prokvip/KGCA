#ifndef TBASIS_TPROTOCOL
#define TBASIS_TPROTOCOL
#include "TMath.h"

enum class CONNECT_TYPE
{
	C_NOCONNECT,		//UMETA(DisplayName = "NoConnect"),
	C_CONNECT,		//UMETA(DisplayName = "Connect"),
	C_LOGIN,			//UMETA(DisplayName = "Login"),
	C_LOGOUT,		//UMETA(DisplayName = "Logout"),
	C_LOBBYLOGIN,	//UMETA(DisplayName = "Lobby-Login"),
	C_LOBBYLOGOUT,	//UMETA(DisplayName = "Lobby-Logout"),
	C_GAMEPLAYING,	//UMETA(DisplayName = "GamePlaying"),
	C_OK_VERIFICATION, //UMETA(DisplayName = "OK_VERIFICATION"),
	C_NO_VERIFICATION, //UMETA(DisplayName = "FAILED_VERIFICATION"),
};


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
		int iSize = sizeof(tGUID);
		out.write(reinterpret_cast<const char*>(&h), iSize);
		return out;
	}
	
	friend std::istream& operator>>(std::istream& in, tGUID& h)
	{
		in.read(reinterpret_cast<char*>(&h), sizeof(tGUID));
		return in;
	}
};
struct GUIDComparer
{
	bool operator()(const tGUID & Left, const tGUID & Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
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
	bool	bAlive;     // 리스폰 가능여부(중복체크용)
	bool	bAttack;    // 공격 가능여부(중복체크용)
	bool	bDamage;	// 데미지 가능여부(중복체크용)
}TPACKET_CHARACTER_INFO;

//------------------------------------------------------------------------------
typedef struct
{
	WORD		Year;
	BYTE		Month, Day;
	BYTE		Hour, Min, Sec;
	BYTE		Sex;				// 성별. 0: 남자 1: 여자
	BYTE		Union;				// 소속된 연합 번호. 

} UNIQUE_VARS;
//------------------------------------------------------------------------------
typedef struct
{
	WORD		Year;
	BYTE		Month, Day;
	BYTE		Hour, Min;
}TUSER_TIME;

//------------------------------------------------------------------------------
typedef struct
{
	WORD		Point;				// 미분배 포인트.
	WORD		STR;				// 힘.
	WORD		INT;				// 지능.
	WORD		DEX;				// 민첩.
	WORD		CHA;				// 카리스마.
	WORD		Life;				// 현재 생명력. (힘 종속)
	WORD		Stamina;			// 현재 지구력. (힘 종속)
	BYTE		Status;				// 현재 상태 값.
	WORD		Skins[6];			// 현재 모습 상태.(0:머리, 1:얼굴)
	BYTE		Parts[4];			// 부위별 피해 상황. 
	BYTE		Level;				// 레벨.
	DWORD		Exp;				// 경험치.
	DWORD		Skills[16];			// 스킬 숙련도. (0->10,000)

}TSTATUS_VARS;

//------------------------------------------------------------------------------
typedef struct
{
	BYTE		Rank;				// 계급.
	BYTE		Medals;				// 획득한 훈장 수.
	WORD		Fame;				// 명성 수치.
	DWORD		Win;				// 승.
	DWORD		Lose;				// 패.

}TSCORE_VARS;

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
	in.read(reinterpret_cast<char*>(&h.ph), sizeof(PACKET_HEADER));
	in.read(h.msg, h.ph.len - PACKET_HEADER_SIZE);
	return in;
}
//static std::istream& operator>>(std::istream& in, UPACKET& h)
//{
//	UPACKET uPacket;
//	in.read(reinterpret_cast<char*>(&uPacket.ph), sizeof(PACKET_HEADER));
//	in.read(uPacket.msg, uPacket.ph.len - PACKET_HEADER_SIZE);
//	h = std::move(uPacket);
//	return in;
//}

static std::stringstream MakePacket(int iType, stringstream& data)
{
	stringstream SendStream;
	PACKET_HEADER header;
	header.iotype = 777;
	header.type = iType;
	header.len = (uint16_t)(PACKET_HEADER_SIZE + data.str().length());
	SendStream << header;
	SendStream << data.str();
	return std::move(SendStream);
}

#endif