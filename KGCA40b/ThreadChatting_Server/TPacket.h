#pragma once
#include "TProtocol.h"
#include <string>

// TPacket data;
// data << header << index << name << damage << mesage;
// data >> header >> index >> name >> damage >> mesage;
class TPacket
{
public:
	char*	m_pOffset;
	char* c_str() { return m_pOffset; }
	UPACKET m_uPacket;
	void PutData(const char* pData, int iSize);
	void GetData(const char* pData, int iSize);
public:
	TPacket& operator << (int data);
	TPacket& operator << (long data);
	TPacket& operator << (short data);
	TPacket& operator << (float data);
	TPacket& operator << (byte data);
	TPacket& operator << (char* data);
	TPacket& operator << (DWORD data);
	TPacket& operator << (std::string data);
public:
	TPacket& operator >> (int& data);
	TPacket& operator >> (long& data);
	TPacket& operator >> (short& data);
	TPacket& operator >> (float& data);
	TPacket& operator >> (byte& data);
	TPacket& operator >> (char* data);
	TPacket& operator >> (DWORD& data);
	TPacket& operator >> (std::string& data);
public:
	TPacket();
	TPacket(WORD iType);
	TPacket(const TPacket& packet);
	~TPacket();
};

