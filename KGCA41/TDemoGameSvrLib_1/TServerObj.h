#pragma once
#include "TServerStd.h"

#define DECLARE_CLASSNAME		static char lpszClassName[]
#define IMPLEMENT_CLASSNAME(s)	char s##::lpszClassName[] = (#s);

#define DECLARE_DYNCREATE		static TObject* CreateObject();
#define IMPLEMENT_DYNCREATE(s)	TObject* s::CreateObject()	{return new s;}

#define DECLARE_DYNAMIC(s)		static CRuntimeClass class##s;
#define IMPLEMENT_DYNAMIC(s)	CRuntimeClass s::class##s={ #s, sizeof(s), s::CreateObject }

#define DECLARE_DYNCLASS_CREATE CRuntimeClass* GetRuntimeClass() const;
#define IMPLEMENT_DYNCLASS_CREATE(s) CRuntimeClass* s::GetRuntimeClass() const{return &class##s;}

#define RUNTIME_CLASS(s) &s::class##s
#define RUNTIME_DECLARE(s)		DECLARE_DYNAMIC(s); DECLARE_DYNCREATE; DECLARE_DYNCLASS_CREATE;
#define RUNTIME_IMPLEMENT(s)	IMPLEMENT_DYNCREATE(s);	IMPLEMENT_DYNAMIC(s); IMPLEMENT_DYNCLASS_CREATE(s);

class TObject;
struct CRuntimeClass
{
	char m_lpszClassName[21];
	int  m_iObjectSize;
	TObject* (*pfnCreateObject)();
	TObject* CreateObject()
	{
		return (*pfnCreateObject)();
	}
};
class TObject
{
public:
	DECLARE_DYNAMIC(TObject);
	DECLARE_CLASSNAME;
	virtual CRuntimeClass* GetRuntimeClass() const
	{
		return NULL;
	}
	virtual ~TObject() {}
protected:
	TObject() {}
};

class TServerObj : public TObject
{
public:
	CRITICAL_SECTION   m_CS;
	volatile long 	m_iLockCheck;
public:
	TServerObj(void);
	virtual ~TServerObj(void);
};
