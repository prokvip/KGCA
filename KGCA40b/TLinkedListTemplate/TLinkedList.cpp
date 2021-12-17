#include "TLinkedList.h"
template<class T>
TNode<T>* TLinkedList<T>::operator++(void) // ++a
{
	m_pCurrent = m_pCurrent->m_pPrev;
	return m_pCurrent;
}
template<class T>
TNode<T>* TLinkedList<T>::operator++(int) // a++
{
	m_pCurrent = m_pCurrent->m_pNext;
	return m_pCurrent;
}
template<class T>
TNode<T>* TLinkedList<T>::begin()
{
	m_pCurrent = m_pHead->m_pNext;
	return m_pHead->m_pNext;
}
template<class T>
TNode<T>* TLinkedList<T>::end()
{
	return m_pTail;
}
template<class T>
TNode<T>* TLinkedList<T>::rbegin()
{
	m_pCurrent = m_pTail->m_pPrev;
	return m_pCurrent;
}
template<class T>
TNode<T>* TLinkedList<T>::rend()
{
	return m_pHead;
}
template<class T>
TNode<T>* TLinkedList<T>::erase(TNode<T>* pNode)
{
	_ASSERT(m_pHead&& m_pTail&& pNode);
	// head<--->1<----->2<----->3<------>tail	
	// head<--->1<----->3<------>tail
	TNode<T>* pPrev = pNode->m_pPrev;
	TNode<T>* pNext = pNode->m_pNext;
	delete pNode;
	pPrev->m_pNext = pNext;
	pNext->m_pPrev = pPrev;
	return pNext;
}
template<class T>
TNode<T>* TLinkedList<T>::find(int iValue)
{
	for (TNode<T>* pNode = begin();
		pNode != end();
		pNode = pNode->m_pNext)
	{
		if (pNode->m_pData.iValue == iValue)
		{
			return pNode;
		}
	}
	return nullptr;
}
template<class T>
void   TLinkedList<T>::clear()
{
	TNode<T>* pDelNode = begin();
	while (pDelNode != end())
	{
		pDelNode = erase(pDelNode);
	}
}

template<class T>
void TLinkedList<T>::insert(TNode<T>* pPosNode, TNode<T>* pNewNode)
{
	TNode<T>* pLink = pPosNode->m_pNext;
	pPosNode->m_pNext = pNewNode;
	pNewNode->m_pNext = pLink;
	pLink->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPosNode;
}
template<class T>
void   TLinkedList<T>::PushBack(TNode<T>* pNewNode)
{
	// head<------->tail
	// (0)head--->newNode---->tail, 
	// (1)head<---newNode<----tail
	// head<--->1<----->tail	
	// head<--->1<----->2<----->tail	
	TNode<T>* pLink = m_pTail->m_pPrev;
	pLink->m_pNext = pNewNode;
	pNewNode->m_pNext = m_pTail;
	m_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pLink;	
}
template<class T>
void   TLinkedList<T>::PushFront(TNode<T>* pNewNode)
{
	// head<------->tail
	// (0)head--->newNode---->tail, 
	// (1)head<---newNode<----tail
	// head<--->1<----->tail	
	// head<--->2<----->1<----->tail
	/*TNode<T>* pLink = m_pHead->m_pNext;
	m_pHead->m_pNext = pNewNode;
	pNewNode->m_pNext = pLink;
	pLink->m_pPrev = pNewNode;
	pNewNode->m_pPrev = m_pHead;*/
	insert(m_pHead, pNewNode);
}
template<class T>
TLinkedList<T>::TLinkedList()
{
	m_pHead = new TNode<T>;
	m_pTail = new TNode<T>;
	m_pHead->m_pNext = m_pTail;
	m_pTail->m_pPrev = m_pHead;
}
template<class T>
TLinkedList<T>::~TLinkedList()
{
	delete m_pHead;
	delete m_pTail;
	m_pHead = nullptr;
	m_pTail = nullptr;
}