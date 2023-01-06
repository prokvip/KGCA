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
#pragma pack(pop)

#define PACKET_CHAR_MSG   1000

