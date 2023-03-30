// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TProtocol.h"

#define MAX_RECV_SIZE 10000
/**
 * 
 */
class TNETCORE_API TStreamPacket
{
public:
	CRITICAL_SECTION   m_CS;
	std::list<UPACKET> m_PacketList;
	typedef std::list<UPACKET>::iterator m_PackItor;
public:
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	DWORD				m_iStartPos;
	DWORD				m_iWritePos;
	DWORD				m_iReadPos;
public:
	void    Put(char* redvBuffer, int iSize);
	void	Reset();
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};
