#pragma once
#include "TServerStd.h"
class TServerObj
{
public:
	CRITICAL_SECTION m_cs;
	HANDLE			 m_hKillEvent;
public:
	TServerObj();
	virtual ~TServerObj();
};

