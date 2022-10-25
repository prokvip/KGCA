#pragma once
#include "TBaseObject.h"
#include "TCameraDebug.h"
class TMap : public TBaseObject
{
public:
	DWORD        m_dwNumRows;
	DWORD        m_dwNumColumns;
	DWORD_VECTOR indexlist;
public:
	bool  Build(UINT iWidth, UINT iHeight);
	bool  UpdateBuffer(TCameraDebug* pMainCamera);
};

