#pragma once
#include <windows.h>
#include <iostream>

#pragma pack(push, 1)
typedef struct {
	WORD iotype;
	WORD len;
	WORD type;
	DWORD time;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[4096];
}UPACKET, * P_UPACKET;

struct TChatMsg
{
	long	index;
	char	name[20]; // ȫ�浿	
	char	message[256];// �ȳ��ϼ���.	
};

struct TLoginReq
{
	char szID[20];
	char szPS[20];
};
struct TLoginAck
{
	int iResult;// 1=����, -1=ID����. -2=PS����.
};

#pragma pack(pop)

#define PACKET_HEADER_SIZE	 10
#define PACKET_TLoginReq_Size	 40
#define PACKET_TLoginAck_Size	 4
struct TSTATUS_VARS
{
	uint16_t	Point;				// �̺й� ����Ʈ.
	uint16_t	STR;				// ��.
	uint16_t	INT;				// ����.
	uint16_t	DEX;				// ��ø.
	uint16_t	CHA;				// ī������.
	uint16_t	Life;				// ���� �����. (�� ����)
	uint16_t	Stamina;			// ���� ������. (�� ����)
	int8_t		Status;				// ���� ���� ��.
	uint16_t	Skins[6];			// ���� ��� ����.(0:�Ӹ�, 1:��)
	int8_t		Parts[4];			// ������ ���� ��Ȳ. 
	int8_t		Level;				// ����.
	uint32_t	Exp;				// ����ġ.
	uint32_t	Skills[16];			// ��ų ���õ�. (0->10,000)

};

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000, // ��ȭ�� �Է¿�û	
	PACKET_CHAT_NAME_ACK, // ��ȭ�� �Է� ����
	PACKET_CHAT_MSG, // ���� ä�� �޼���.
	PACKET_LOGIN_REQ, // �α��ο�û
	PACKET_LOGOUT_REQ, // �α׾ƿ���û
	PACKET_LOGIN_ACK, // �α�������
	PACKET_LOGOUT_ACK, // �α׾ƿ�����
	PACKET_LOGOUT_PLAYER, // �α׾ƿ� �÷��̾�

	PACKET_CREATE_ACCOUNT_REQ = 2000, // ����������û
	PACKET_DELETE_ACCOUNT_REQ, // ����������û
	PACKET_CREATE_ACCOUNT_ACK, // ������������
	PACKET_DELETE_ACCOUNT_ACK, // ������������

	PACKET_USER_POSITION = 3000, // �ɸ��� ��ġ �̵� ����
	PACKET_STOP_POSITION,// �ɸ��� ����
	PACKET_LOBBY_USER_INFO, // ���� ��������Ʈ ����


	PACKET_CREATE_CHARACTER_REQ = 4000, // �ɸ��� ���� ��û
	PACKET_DELETE_CHARACTER_REQ, // �ɸ��� ���� ��û
	PACKET_CREATE_CHARACTER_ACK, // �ɸ��� ���� ����
	PACKET_DELETE_CHARACTER_ACK, // �ɸ��� ���� ����

	PACKET_HIT_CHARACTER = 5000, // �ɸ��� �ǰ� ����
	PACKET_HIT_NPC, // NPC �ǰ� ����
	PACKET_ATTACK_CHARACTER, // �ɸ��� ���� ����
	PACKET_ATTACK_NPC, // NPC ���� ����
	PACKET_DEAD_CHARACTER, // �ɸ��� ����
	PACKET_DEAD_NPC, // NPC ����
	PACKET_DAMAGE_CHARACTER, // �ɸ��� ������ ����
	PACKET_DAMAGE_NPC, // NPC ������ ����
	PACKET_SPAWN_CHARACTER, // �ɸ��� ����
	PACKET_SPAWN_NPC, // NPC ����
	PACKET_SPAWN_NPC_LIST, // 
	PACKET_SYNC_CHARACTER, // �ɸ��� ����ȭ
	PACKET_SYNC_NPC_LIST, // NPC ����ȭ	
	PACKET_NPC_TARGETMOVE,  // NPC Ÿ���̵�	


	PACKET_ZONE_ENTRY_REQ = 6000,//������ ���� ��û
	PACKET_ZONE_ENTRY_ACK, // ���� �� ���� ����
	PACKET_ZONE_NEW_PLAYER, // ���� �� ���� �ɸ��� ���� ����
	PACKET_ZONE_PLAYERS_INFO, // ���� ���� ������ �������� �÷��� �ɸ��� ���� ���� 
	PACKET_ZONE_USER_INFO,// ������ ��������Ʈ ����(�÷��̾�+��Ÿ(������ ��))
	PACKET_ZONE_TARGET_REQ, // �� Ÿ�� ��ġ ��û  
	PACKET_ZONE_TARGET_ACK, // �� Ÿ�� ��ġ ����
};
/*
* msg = "�ȳ�";
* UPACKET sendPacket;
* sendPacket.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
* sendPacket.ph.type = PACKET_CHAT_MSG;
* sendPacket.msg = msg;
*/

/*
* TChatMsg msg;
* msg.name = "ȫ�浿";
* msg.message = "�ȳ��ϼ���.";
* UPACKET sendPacket;
* sendPacket.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
* sendPacket.ph.type = PACKET_CHAT_MSG;
* sendPacket.msg = msg;
*/