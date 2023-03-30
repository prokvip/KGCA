#pragma once
#include "TServerObj.h"
class TQueue : public TServerObj
{
public:
	std::queue<TServerObj*>  m_List;
public:
	void Push(TServerObj* pObj);
	TServerObj* Pop();
public:
	TQueue();
	virtual ~TQueue();
};

