#include "TServerObj.h"
TServerObj::TServerObj()
{
	InitializeCriticalSection(&m_cs);
}
TServerObj::~TServerObj()
{
	DeleteCriticalSection(&m_cs);
}