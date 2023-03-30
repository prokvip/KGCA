#pragma once
#include "TServerObj.h"
#include "TProtocol.h"

class TTargetPointMgr
{
private:
	std::map<int, TPACKET_TARGETPOINT_INFO> m_tpMap;
public:
	std::map<int, TPACKET_TARGETPOINT_INFO>& Get();
public:	
	friend ostream& operator<<(ostream &stream, TTargetPointMgr& tpList);
	friend istream& operator>>(istream& stream, TTargetPointMgr& tpList);
public:
	TTargetPointMgr();
	virtual ~TTargetPointMgr();
};

