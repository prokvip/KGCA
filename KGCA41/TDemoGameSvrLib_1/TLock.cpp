#include "TLock.h"
#include "TDebugString.h"

TLock::TLock(TServerObj* pSvrObj): m_pServerObj( pSvrObj )
{
	if( m_pServerObj ==NULL ) return;
	EnterCriticalSection( &m_pServerObj->m_CS );
}

TLock::~TLock(void)
{
	if( m_pServerObj==NULL ) return;
	LeaveCriticalSection( &m_pServerObj->m_CS );
}
