#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
template < class T>
class TNode
{
public:
	T m_pData;
	TNode* m_pNext;
	TNode* m_pPrev;
	TNode() {
		m_pNext = m_pPrev = nullptr;
	}
	~TNode() {		
	}
};

template<class T>
class TLinkedList
{
public:
	TNode<T>* m_pHead = NULL;
	TNode<T>* m_pTail = NULL;
	TNode<T>* m_pNext = NULL;
	TNode<T>* m_pCurNode = NULL;
public:
	static int	  m_iCounter;
public:
	TNode<T>* operator ++(void);
	TNode<T>* operator ++(int);
	TNode<T>* operator [](int);
public:
	TNode<T>*	begin();
	TNode<T>*	end();
	TNode<T>*	next();
	TNode<T>*	rbegin();
	TNode<T>*	rend();
	TNode<T>*	rnext();
	int			size();
public:
	void		clear();	
	void		push_back(T pData);
	void		push_front(T pData);
	void		push_back(T , TNode<T>* );
	void		push_front(T pData, TNode<T>* pSelectData);
	TNode<T>*   erase(TNode<T>* pNode);
	T			Find(T data);	
public:
	TLinkedList();
	~TLinkedList();
};
template<class T>
int	  TLinkedList<T>::m_iCounter = 0;

template<class T>
TNode<T>* TLinkedList<T>::operator ++(void)
{
	TNode<T>* pTemp = m_pCurNode->m_pNext;
	m_pCurNode = pTemp;
	return pTemp;
}
template<class T>
TNode<T>* TLinkedList<T>::operator ++(int)
{
	return nullptr;
}
template<class T>
TNode<T>* TLinkedList<T>::operator [](int index)
{
	TNode<T>* pSave = m_pCurNode;
	TNode<T>* pCur = begin();
	for (int iCnt = 0; iCnt < index; iCnt++)
	{
		pCur = next();
	}
	m_pCurNode = pSave;
	return pCur;
}
template<class T>
int		TLinkedList<T>::size()
{
	return m_iCounter;
}
template<class T>
TLinkedList<T>::~TLinkedList()
{
	delete m_pHead;
	delete m_pTail;
}
template<class T>
TLinkedList<T>::TLinkedList()
{
	m_pHead = new TNode<T>;
	m_pTail = new TNode<T>;
	m_pHead->m_pNext = m_pTail;
	m_pHead->m_pPrev = NULL;
	m_pTail->m_pNext = NULL;
	m_pTail->m_pPrev = m_pHead;
	m_pNext = m_pHead;
}
template<class T>
void   TLinkedList<T>::push_back(T pNewData)
{
	_ASSERT(pNewData);
	TNode<T>* pNewNode = new TNode<T>();
	pNewNode->m_pData = pNewData;
	//   pPrev -> m_pTail ==>  pPrev -> pNewNode -> m_pTail
	TNode<T>* pPrev = m_pTail->m_pPrev;
	m_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPrev;

	pPrev->m_pNext = pNewNode;
	pNewNode->m_pNext = m_pTail;
	m_iCounter++;
}
//// pLinkNode != NULL �϶��� pLinkNode->m_pNext= pNewNode ���Եȴ�.
//// pLinkNode == NULL �϶��� m_pNext->m_pNext = pNewNode ���Եȴ�.
template<class T>
void   TLinkedList<T>::push_back(T pNewData, TNode<T>* pLinkNode)
{
	_ASSERT(pNewData);
	TNode<T>* pNewNode = new TNode<T>();
	pNewNode->m_pData = pNewData;

	_ASSERT(pLinkNode);
	_ASSERT(pLinkNode != m_pTail);
	if (pLinkNode == NULL)
	{
		push_back(pNewNode);
	}
	else
	{
		// pLinkNode <-> pNext => pLinkNode<->pNewNode<->pNext;
		TNode<T>* pNext = pLinkNode->m_pNext;
		pLinkNode->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;
		pNext->m_pPrev = pNewNode;
		pNewNode->m_pPrev = pLinkNode;
		m_iCounter++;
	}

}
// pLinkNode != NULL �϶��� pLinkNode->m_pNext= pNewNode ���Եȴ�.
// pLinkNode == NULL �϶��� m_pHead->m_pNext = pNewNode ���Եȴ�.
template<class T>
void   TLinkedList<T>::push_front(T pNewData)
{
	_ASSERT(pNewData);
	TNode<T>* pNewNode = new TNode<T>();
	pNewNode->m_pData = pNewData;

	// m_pHead -> pNext =>  m_pHead->pNewNode->pNext
	TNode<T>* pNext = m_pHead->m_pNext;
	m_pHead->m_pNext = pNewNode;
	pNewNode->m_pNext = pNext;

	pNext->m_pPrev = pNewNode;
	pNewNode->m_pPrev = m_pHead;

	m_iCounter++;
}
template<class T>
void   TLinkedList<T>::push_front(T pNewData, TNode<T>* pLinkNode)
{
	_ASSERT(pNewData);
	TNode<T>* pNewNode = new TNode<T>();
	pNewNode->m_pData = pNewData;

	_ASSERT(pLinkNode);
	_ASSERT(pLinkNode != m_pHead);
	if (pLinkNode == NULL)
	{
		push_front(pNewNode);
	}
	else
	{
		// pPrev <-> pLinkNode => pPrev<->pNewNode<->pLinkNode;
		TNode<T>* pPrev = pLinkNode->m_pPrev;

		pPrev->m_pNext = pNewNode;
		pNewNode->m_pNext = pLinkNode;
		pLinkNode->m_pPrev = pNewNode;
		pNewNode->m_pPrev = pPrev;

		m_iCounter++;
	}
}

template<class T>
T TLinkedList<T>::Find(T data)
{
	for (TNode<T>* pNode = m_pHead->m_pNext;
		pNode != m_pTail;
		pNode = pNode->m_pNext)
	{
		if (pNode->m_pData == data)
		{
			return pNode->m_pData;
		}
	}
	return nullptr;
}

template<class T>
TNode<T>* TLinkedList<T>::begin() 
{
	m_pCurNode = m_pHead->m_pNext;
	return m_pHead->m_pNext;
}
template<class T>
TNode<T>* TLinkedList<T>::end() { return m_pTail; }
template<class T>
TNode<T>* TLinkedList<T>::next() 
{
	TNode<T>* pTemp = m_pCurNode->m_pNext;
	m_pCurNode = pTemp;
	return pTemp;
}
template<class T>
TNode<T>* TLinkedList<T>::rbegin() 
{
	m_pCurNode = m_pTail->m_pPrev;
	return m_pTail->m_pPrev;
}
template<class T>
TNode<T>* TLinkedList<T>::rend() { return m_pHead; }
template<class T>
TNode<T>* TLinkedList<T>::rnext() 
{
	TNode<T>* pTemp = m_pCurNode->m_pPrev;
	m_pCurNode = pTemp;
	return pTemp;
}
template<class T>
TNode<T>*   TLinkedList<T>::erase(TNode<T>* pNode)
{
	_ASSERT(pNode);
	_ASSERT(pNode != m_pHead);
	_ASSERT(pNode != m_pTail);
	TNode<T>* pPrev = pNode->m_pPrev;
	TNode<T>* pNext = pNode->m_pNext;
	free(pNode);
	pNode = nullptr;
	pPrev->m_pNext = pNext;
	pNext->m_pPrev = pPrev;
	return pNext;
}
template<class T>
void TLinkedList<T>::clear()
{
	TNode<T>* pNode = m_pHead->m_pNext;
	while (pNode != m_pTail)
	{
		TNode<T>* pNext = pNode->m_pNext;
		free(pNode);
		pNode = pNext;
	}
}
//template<class T>
//void    TLinkedList<T>::sort()
//{
//	// 1, ����Ʈ���� ���� ���� ��� �˻��Ѵ�.
//	// 2, �Ǿտ� �߰��Ѵ�. ���� ������ �̵��Ѵ�.
//	// 3, (������ �߰��� Next)�ݺ��Ѵ�.
//	// 4, �߰������ �̵����Ŀ� pNext==m_pTail
//
//	// 1, ������ ����, ���� ���� ��� -> ��� -> ����
//	// 2, �ݺ� (m_pHead->pNext== m_pTail)
//	// 3, ���� �ε�.
//
//	// 1, ��2 ���Ḯ��Ʈ ����
//	// 2, ���� ���� ��� -> ��2 ���Ḯ��Ʈ �߰� -> ����
//	// 2, �ݺ� (m_pHead->pNext== m_pTail)
//}