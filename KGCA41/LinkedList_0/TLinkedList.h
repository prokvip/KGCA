#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

template<class T>
class TLinkedList
{
protected:
	T* m_pHead = NULL;
	T* m_pTail = NULL;
	T* m_pNext = NULL;
	T* m_pCurNode = NULL;
public:
	static int	  m_iCounter;
public:
	T* operator ++(void);
	T* operator ++(int);
	T* operator [](int);
public:
	T*	begin();
	T*	end();
	T*	next();
	T*	rbegin();
	T*	rend();
	T*	rnext();
	int			size();
public:
	void		clear();	
	void		push_back(T* pNewNode);
	void		push_front(T* pNewNode);
	void		push_back(T* pNewNode, T* pLinkNode);
	void		push_front(T* pNewNode, T* pLinkNode);
	void		erase(T* pNode);
	T*	Find(int iAge);
	void		sort();
public:
	friend std::ostream& operator << (std::ostream& os, TLinkedList& list);
	friend std::istream& operator >> (std::istream& is, TLinkedList& list);
public:
	TLinkedList();
	~TLinkedList();
};
template<class T>
int	  TLinkedList<T>::m_iCounter = 0;
template<class T>
std::ostream& operator << (std::ostream& os, TLinkedList<T>& list)
{
	for (T* pNode = list.begin();
		pNode != list.end();
		pNode = list.next())
	{
		os << *pNode;
	}
	return os;
}
template<class T>
std::istream& operator >> (std::iostream& is, TLinkedList<T>& list)
{
	return is;
}
template<class T>
T* TLinkedList<T>::operator ++(void)
{
	T* pTemp = m_pCurNode->m_pNext;
	m_pCurNode = pTemp;
	return pTemp;
}
template<class T>
T* TLinkedList<T>::operator ++(int)
{
	return nullptr;
}
template<class T>
T* TLinkedList<T>::operator [](int index)
{
	T* pSave = m_pCurNode;
	T* pCur = begin();
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
	m_pHead = new T;
	m_pTail = new T;
	m_pHead->m_pNext = m_pTail;
	m_pHead->m_pPrev = NULL;
	m_pTail->m_pNext = NULL;
	m_pTail->m_pPrev = m_pHead;
	m_pNext = m_pHead;
}
template<class T>
void   TLinkedList<T>::push_back(T* pNewNode)
{
	_ASSERT(pNewNode);
	//   pPrev -> m_pTail ==>  pPrev -> pNewNode -> m_pTail
	T* pPrev = m_pTail->m_pPrev;
	m_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPrev;

	pPrev->m_pNext = pNewNode;
	pNewNode->m_pNext = m_pTail;
	m_iCounter++;
}
// pLinkNode != NULL 일때는 pLinkNode->m_pNext= pNewNode 삽입된다.
// pLinkNode == NULL 일때는 m_pNext->m_pNext = pNewNode 삽입된다.
template<class T>
void   TLinkedList<T>::push_back(T* pNewNode, T* pLinkNode)
{
	_ASSERT(pLinkNode);
	_ASSERT(pLinkNode != m_pTail);
	if (pLinkNode == NULL)
	{
		push_back(pNewNode);
	}
	else
	{
		// pLinkNode <-> pNext => pLinkNode<->pNewNode<->pNext;
		T* pNext = pLinkNode->m_pNext;
		pLinkNode->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;
		pNext->m_pPrev = pNewNode;
		pNewNode->m_pPrev = pLinkNode;
		m_iCounter++;
	}

}
// pLinkNode != NULL 일때는 pLinkNode->m_pNext= pNewNode 삽입된다.
// pLinkNode == NULL 일때는 m_pHead->m_pNext = pNewNode 삽입된다.
template<class T>
void   TLinkedList<T>::push_front(T* pNewNode)
{
	// m_pHead -> pNext =>  m_pHead->pNewNode->pNext
	T* pNext = m_pHead->m_pNext;
	m_pHead->m_pNext = pNewNode;
	pNewNode->m_pNext = pNext;

	pNext->m_pPrev = pNewNode;
	pNewNode->m_pPrev = m_pHead;

	m_iCounter++;
}
template<class T>
void   TLinkedList<T>::push_front(T* pNewNode, T* pLinkNode)
{
	_ASSERT(pLinkNode);
	_ASSERT(pLinkNode != m_pHead);
	if (pLinkNode == NULL)
	{
		push_front(pNewNode);
	}
	else
	{
		// pPrev <-> pLinkNode => pPrev<->pNewNode<->pLinkNode;
		T* pPrev = pLinkNode->m_pPrev;

		pPrev->m_pNext = pNewNode;
		pNewNode->m_pNext = pLinkNode;
		pLinkNode->m_pPrev = pNewNode;
		pNewNode->m_pPrev = pPrev;

		m_iCounter++;
	}
}
template<class T>
T* TLinkedList<T>::Find(int iAge)
{
	for (T* pNode = m_pHead->m_pNext;
		pNode != m_pTail;
		pNode = pNode->m_pNext)
	{
		if (pNode->m_iAge == iAge)
		{
			return pNode;
		}
	}
	return nullptr;
}
template<class T>
T* TLinkedList<T>::begin() {
	m_pCurNode = m_pHead->m_pNext;
	return m_pHead->m_pNext;
}
template<class T>
T* TLinkedList<T>::end() { return m_pTail; }
template<class T>
T* TLinkedList<T>::next() {
	T* pTemp = m_pCurNode->m_pNext;
	m_pCurNode = pTemp;
	return pTemp;
}
template<class T>
T* TLinkedList<T>::rbegin() {
	m_pCurNode = m_pTail->m_pPrev;
	return m_pTail->m_pPrev;
}
template<class T>
T* TLinkedList<T>::rend() { return m_pHead; }
template<class T>
T* TLinkedList<T>::rnext() {
	T* pTemp = m_pCurNode->m_pPrev;
	m_pCurNode = pTemp;
	return pTemp;
}
template<class T>
void   TLinkedList<T>::erase(T* pNode)
{
	_ASSERT(pNode);
	_ASSERT(pNode != m_pHead);
	_ASSERT(pNode != m_pTail);
	T* pPrev = pNode->m_pPrev;
	T* pNext = pNode->m_pNext;
	free(pNode);
	pNode = nullptr;
	pPrev->m_pNext = pNext;
	pNext->m_pPrev = pPrev;
}
template<class T>
void TLinkedList<T>::clear()
{
	T* pNode = m_pHead->m_pNext;
	while (pNode != m_pTail)
	{
		T* pNext = pNode->m_pNext;
		free(pNode);
		pNode = pNext;
	}
}
template<class T>
void    TLinkedList<T>::sort()
{
	// 1, 리스트에서 가장 작은 노드 검색한다.
	// 2, 맨앞에 추가한다. 기존 데이터 이동한다.
	// 3, (직전에 추가된 Next)반복한다.
	// 4, 추가노드의 이동이후에 pNext==m_pTail

	// 1, 빈파일 생성, 가장 작은 노드 -> 출력 -> 삭제
	// 2, 반복 (m_pHead->pNext== m_pTail)
	// 3, 파일 로드.

	// 1, 제2 연결리스트 생성
	// 2, 가장 작은 노드 -> 제2 연결리스트 추가 -> 삭제
	// 2, 반복 (m_pHead->pNext== m_pTail)
}