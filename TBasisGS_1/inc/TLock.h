#pragma once
#include "TServerobj.h"

class TLock 
{
private:
	TServerObj*		m_pServerObj;	
public:
	TLock( TServerObj* pSvrObj );
	virtual ~TLock(void);
};
