#pragma once
#include <windows.h>

#pragma pack(push, 1)
typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[4096];
}UPACKET, *P_UPACKET;

struct TChatMsg
{	
	long	index;
	char	name[20]; // È«±æµ¿
	short	damage;
	char	message[256];// ¾È³çÇÏ¼¼¿©.	
};
#pragma pack(pop)

#define PACKET_HEADER_SIZE	 4
#define PACKET_CHAT_MSG      1000
/*
* msg = "¾È³ç";
* UPACKET sendPacket;
* sendPacket.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
* sendPacket.ph.type = PACKET_CHAT_MSG;
* sendPacket.msg = msg;
*/

/*
* TChatMsg msg;
* msg.name = "È«±æµ¿";
* msg.message = "¾È³çÇÏ¼¼¿©.";
* UPACKET sendPacket;
* sendPacket.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
* sendPacket.ph.type = PACKET_CHAT_MSG;
* sendPacket.msg = msg;
*/