#include "TTargetPointMgr.h"

std::map<int, TPACKET_TARGETPOINT_INFO>& TTargetPointMgr::Get()
{
	return m_tpMap;
}
ostream& operator<<(ostream &stream, TTargetPointMgr& tpList)
{
	size_t iNumCount = tpList.m_tpMap.size();
	tBlockWrite<size_t>(stream, iNumCount);
	//stream << tpList.m_tpMap.size();
	for (auto& tp : tpList.m_tpMap)
	{
		tBlockWrite<TPACKET_TARGETPOINT_INFO>(stream, tp.second);
	}
	return stream;
}
istream& operator>>(istream& stream, TTargetPointMgr& tpList)
{
	tpList.m_tpMap.clear();	
	size_t iNumCount = 0;
	tBlockRead<size_t>(stream, iNumCount);

	TPACKET_TARGETPOINT_INFO tp;
	for (int iIndex = 0; iIndex < iNumCount; iIndex++)
	{
		tBlockRead<TPACKET_TARGETPOINT_INFO>(stream,tp);
		tpList.m_tpMap[iIndex] = tp;
	}
	return stream;
}

TTargetPointMgr::TTargetPointMgr()
{
}


TTargetPointMgr::~TTargetPointMgr()
{
}
