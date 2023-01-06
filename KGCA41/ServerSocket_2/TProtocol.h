#pragma once
#define PACKET_HEADER_SIZE 4
#pragma pack(push, 1)
	typedef struct
	{
		short len;		
		short type;
	}PACKET_HEADER;

	typedef struct
	{
		PACKET_HEADER ph;
		char   msg[2048];
	}UPACKET;

	///   케릭터 생성 패킷
	typedef struct {
		byte    szName[13];
		int		iIndex;
		int		iData[50];
	}CHARACTER_INFO;
	///   아이템 생성 패킷
	typedef struct {
		byte    szName[13];
		int		iIndex;
		int		iData[10];
	}ITEM_INFO;
#pragma pack(pop)

#define PACKET_CHAR_MSG   1000
#define PACKET_GAME_START    2000
#define PACKET_GAME_END    3000
#define PACKET_CREATE_CHARACTER    4000
#define PACKET_MOVE_CHARACTER    5000

