#include "TLinkedList.h"
TNode* TLinkedList::operator++(void) // ++a
{
	m_pCurrent = m_pCurrent->m_pPrev;
	return m_pCurrent;
}
TNode* TLinkedList::operator++(int) // a++
{
	m_pCurrent = m_pCurrent->m_pNext;
	return m_pCurrent;
}

TNode* TLinkedList::begin()
{
	m_pCurrent = m_pHead->m_pNext;
	return m_pHead->m_pNext;
}
TNode* TLinkedList::end()
{
	return m_pTail;
}
TNode* TLinkedList::rbegin()
{
	m_pCurrent = m_pTail->m_pPrev;
	return m_pCurrent;
}
TNode* TLinkedList::rend()
{
	return m_pHead;
}
TNode* TLinkedList::erase(TNode* pNode)
{
	_ASSERT(m_pHead&& m_pTail&& pNode);
	// head<--->1<----->2<----->3<------>tail	
	// head<--->1<----->3<------>tail
	TNode* pPrev = pNode->m_pPrev;
	TNode* pNext = pNode->m_pNext;
	delete pNode;
	pPrev->m_pNext = pNext;
	pNext->m_pPrev = pPrev;
	return pNext;
}
TNode* TLinkedList::find(int iValue)
{
	for (TNode* pNode = begin();
		pNode != end();
		pNode = pNode->m_pNext)
	{
		if (pNode->m_iValue == iValue)
		{
			return pNode;
		}
	}
	return nullptr;
}
void   TLinkedList::clear()
{
	TNode* pDelNode = begin();
	while (pDelNode != end())
	{
		pDelNode = erase(pDelNode);
	}
}

void TLinkedList::insert(TNode* pPosNode, TNode* pNewNode)
{
	TNode* pLink = pPosNode->m_pNext;
	pPosNode->m_pNext = pNewNode;
	pNewNode->m_pNext = pLink;
	pLink->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPosNode;
}
void   TLinkedList::PushBack(TNode* pNewNode)
{
	// head<------->tail
	// (0)head--->newNode---->tail, 
	// (1)head<---newNode<----tail
	// head<--->1<----->tail	
	// head<--->1<----->2<----->tail	
	TNode* pLink = m_pTail->m_pPrev;
	pLink->m_pNext = pNewNode;
	pNewNode->m_pNext = m_pTail;
	m_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pLink;	
}
void   TLinkedList::PushFront(TNode* pNewNode)
{
	// head<------->tail
	// (0)head--->newNode---->tail, 
	// (1)head<---newNode<----tail
	// head<--->1<----->tail	
	// head<--->2<----->1<----->tail
	/*TNode* pLink = m_pHead->m_pNext;
	m_pHead->m_pNext = pNewNode;
	pNewNode->m_pNext = pLink;
	pLink->m_pPrev = pNewNode;
	pNewNode->m_pPrev = m_pHead;*/
	insert(m_pHead, pNewNode);
}

TLinkedList::TLinkedList()
{
	m_pHead = new TNode;
	m_pTail = new TNode;
	m_pHead->m_pNext = m_pTail;
	m_pTail->m_pPrev = m_pHead;
}
TLinkedList::~TLinkedList()
{
	delete m_pHead;
	delete m_pTail;
	m_pHead = nullptr;
	m_pTail = nullptr;
}