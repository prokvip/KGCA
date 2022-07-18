#include "TLinkedList.h"
int	  TLinkedList::m_iCounter = 0;
TStudent* TLinkedList::operator ++(void)
{
	TStudent* pTemp = m_pCurNode->m_pNext;
	m_pCurNode = pTemp;
	return pTemp;
}
TStudent* TLinkedList::operator ++(int)
{
	return nullptr;
}
TStudent* TLinkedList::operator [](int index)
{
	TStudent* pSave = m_pCurNode;
		TStudent* pCur = begin();
		for (int iCnt = 0; iCnt < index; iCnt++)
		{
			pCur = next();
		}
	m_pCurNode = pSave;
	return pCur;
}
int		TLinkedList::size()
{
	return m_iCounter;
}
TLinkedList::~TLinkedList()
{
	delete m_pHead;
	delete m_pTail;
}

TLinkedList::TLinkedList()
{
	m_pHead = new TStudent;
	m_pTail = new TStudent;
	m_pHead->m_pNext = m_pTail;
	m_pHead->m_pPrev = NULL;
	m_pTail->m_pNext = NULL;
	m_pTail->m_pPrev = m_pHead;
	m_pNext = m_pHead;
}

void   TLinkedList::push_back(TStudent* pNewNode)
{
	_ASSERT(pNewNode);
	//   pPrev -> m_pTail ==>  pPrev -> pNewNode -> m_pTail
	TStudent* pPrev = m_pTail->m_pPrev;
	m_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPrev;

	pPrev->m_pNext = pNewNode;
	pNewNode->m_pNext = m_pTail;
	m_iCounter++;
}
// pLinkNode != NULL �϶��� pLinkNode->m_pNext= pNewNode ���Եȴ�.
// pLinkNode == NULL �϶��� m_pNext->m_pNext = pNewNode ���Եȴ�.
void   TLinkedList::push_back(TStudent* pNewNode, TStudent* pLinkNode)
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
		TStudent* pNext = pLinkNode->m_pNext;
		pLinkNode->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;
		pNext->m_pPrev = pNewNode;
		pNewNode->m_pPrev = pLinkNode;
		m_iCounter++;
	}

}
// pLinkNode != NULL �϶��� pLinkNode->m_pNext= pNewNode ���Եȴ�.
// pLinkNode == NULL �϶��� m_pHead->m_pNext = pNewNode ���Եȴ�.
void   TLinkedList::push_front(TStudent* pNewNode)
{
	// m_pHead -> pNext =>  m_pHead->pNewNode->pNext
	TStudent* pNext = m_pHead->m_pNext;
	m_pHead->m_pNext = pNewNode;
	pNewNode->m_pNext = pNext;

	pNext->m_pPrev = pNewNode;
	pNewNode->m_pPrev = m_pHead;

	m_iCounter++;
}

void   TLinkedList::push_front(TStudent* pNewNode, TStudent* pLinkNode)
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
		TStudent* pPrev = pLinkNode->m_pPrev;

		pPrev->m_pNext = pNewNode;
		pNewNode->m_pNext = pLinkNode;
		pLinkNode->m_pPrev = pNewNode;
		pNewNode->m_pPrev = pPrev;

		m_iCounter++;
	}
}
TStudent* TLinkedList::Find(int iAge)
{
	for (TStudent* pNode = m_pHead->m_pNext;
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

TStudent* TLinkedList::begin() {
	m_pCurNode = m_pHead->m_pNext;
	return m_pHead->m_pNext;
}
TStudent* TLinkedList::end() { return m_pTail; }
TStudent* TLinkedList::next() {
	TStudent* pTemp = m_pCurNode->m_pNext;
	m_pCurNode = pTemp;
	return pTemp;
}
TStudent* TLinkedList::rbegin() {
	m_pCurNode = m_pTail->m_pPrev;
	return m_pTail->m_pPrev;
}
TStudent* TLinkedList::rend() { return m_pHead; }
TStudent* TLinkedList::rnext() {
	TStudent* pTemp = m_pCurNode->m_pPrev;
	m_pCurNode = pTemp;
	return pTemp;
}
void   TLinkedList::erase(TStudent* pNode)
{
	_ASSERT(pNode);
	_ASSERT(pNode != m_pHead);
	_ASSERT(pNode != m_pTail);
	TStudent* pPrev = pNode->m_pPrev;
	TStudent* pNext = pNode->m_pNext;
	free(pNode);
	pNode = nullptr;
	pPrev->m_pNext = pNext;
	pNext->m_pPrev = pPrev;
}
void TLinkedList::clear()
{
	TStudent* pNode = m_pHead->m_pNext;
	while (pNode != m_pTail)
	{
		TStudent* pNext = pNode->m_pNext;
		free(pNode);
		pNode = pNext;
	}
}
void    TLinkedList::sort()
{
	// 1, ����Ʈ���� ���� ���� ��� �˻��Ѵ�.
	// 2, �Ǿտ� �߰��Ѵ�. ���� ������ �̵��Ѵ�.
	// 3, (������ �߰��� Next)�ݺ��Ѵ�.
	// 4, �߰������ �̵����Ŀ� pNext==m_pTail

	// 1, ������ ����, ���� ���� ��� -> ��� -> ����
	// 2, �ݺ� (m_pHead->pNext== m_pTail)
	// 3, ���� �ε�.

	// 1, ��2 ���Ḯ��Ʈ ����
	// 2, ���� ���� ��� -> ��2 ���Ḯ��Ʈ �߰� -> ����
	// 2, �ݺ� (m_pHead->pNext== m_pTail)
}