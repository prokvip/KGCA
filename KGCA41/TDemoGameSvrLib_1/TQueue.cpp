#include "TQueue.h"
#include "TLock.h"

void TQueue::Push(TServerObj* pObj)
{
	{
		TLock sync(this);
		m_List.push(pObj);
	}
}
TServerObj* TQueue::Pop()
{
	TServerObj* pObj = nullptr;
	{
		TLock sync(this);
		pObj = (TServerObj*)m_List.front();
		m_List.pop();
	}
	return pObj;
}
TQueue::TQueue()
{
}


TQueue::~TQueue()
{
}
