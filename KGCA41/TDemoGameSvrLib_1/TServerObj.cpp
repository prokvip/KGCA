#include "TServerObj.h"

IMPLEMENT_CLASSNAME(TObject);
CRuntimeClass TObject::classTObject ={ "TObject", sizeof(TObject), NULL };

TServerObj::TServerObj(void)
{
	InitializeCriticalSection(&m_CS);
	m_iLockCheck = 0;
}

TServerObj::~TServerObj(void)
{
	DeleteCriticalSection(&m_CS);
}
