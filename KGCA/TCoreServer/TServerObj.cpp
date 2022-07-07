#include "TServerObj.h"
TServerObj::TServerObj()
{
	InitializeCriticalSection(&m_cs);
	m_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
}
TServerObj::~TServerObj()
{
	DeleteCriticalSection(&m_cs);
	CloseHandle(m_hKillEvent);
}