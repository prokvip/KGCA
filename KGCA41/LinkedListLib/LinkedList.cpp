#include "LinkedList.h"

int g_iCounter = 0;
TStudent* g_pHead = NULL;
TStudent* g_pTail = NULL;
TStudent* g_pNext = NULL;
TStudent* g_pCurNode = NULL;


TStudent* NewNode()
{
	TStudent* pNewNode = (TStudent*)malloc(sizeof(TStudent));
	memset(pNewNode, 0, sizeof(TStudent));
	pNewNode->m_iAge = g_iCounter + 1;
	pNewNode->m_szName[0] = 65 + rand() % 26;
	pNewNode->m_szName[1] = 65 + rand() % 26;
	pNewNode->m_szName[2] = 65 + rand() % 26;
	pNewNode->m_szName[3] = 0;
	pNewNode->m_iKor = 0 + rand() % 101;
	pNewNode->m_iEng = 0 + rand() % 101;
	pNewNode->m_iMat = 0 + rand() % 101;
	pNewNode->m_iTotal = pNewNode->m_iKor +
		pNewNode->m_iEng +
		pNewNode->m_iMat;
	pNewNode->m_fAverage = pNewNode->m_iTotal / 3;
	return pNewNode;
}
void InitData(int iCounter)
{
	for (int iNode = 0; iNode < iCounter; iNode++)
	{
		push_back(NewNode());
	}
	for (int iNode = iCounter; iNode < iCounter * 2; iNode++)
	{
		push_front(NewNode());
	}
}
void Release()
{
	free(g_pHead);
	free(g_pTail);
}

void   Init()
{
	g_pHead = (TStudent*)malloc(sizeof(TStudent));
	g_pTail = (TStudent*)malloc(sizeof(TStudent));
	memset(g_pHead, 0, sizeof(TStudent));
	memset(g_pTail, 0, sizeof(TStudent));

	g_pHead->m_pNext = g_pTail;
	g_pHead->m_pPrev = NULL;
	g_pTail->m_pNext = NULL;
	g_pTail->m_pPrev = g_pHead;

	g_pNext = g_pHead;
}

void   push_back(TStudent* pNewNode)
{
	_ASSERT(pNewNode);
	//   pPrev -> g_pTail ==>  pPrev -> pNewNode -> g_pTail
	TStudent* pPrev = g_pTail->m_pPrev;
	g_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPrev;

	pPrev->m_pNext = pNewNode;
	pNewNode->m_pNext = g_pTail;
	g_iCounter++;
}
// pLinkNode != NULL 일때는 pLinkNode->m_pNext= pNewNode 삽입된다.
// pLinkNode == NULL 일때는 g_pNext->m_pNext = pNewNode 삽입된다.
void   push_back(TStudent* pNewNode, TStudent* pLinkNode)
{
	_ASSERT(pLinkNode);
	_ASSERT(pLinkNode != g_pTail);
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
		g_iCounter++;
	}

}
// pLinkNode != NULL 일때는 pLinkNode->m_pNext= pNewNode 삽입된다.
// pLinkNode == NULL 일때는 g_pHead->m_pNext = pNewNode 삽입된다.
void   push_front(TStudent* pNewNode)
{
	// g_pHead -> pNext =>  g_pHead->pNewNode->pNext
	TStudent* pNext = g_pHead->m_pNext;
	g_pHead->m_pNext = pNewNode;
	pNewNode->m_pNext = pNext;

	pNext->m_pPrev = pNewNode;
	pNewNode->m_pPrev = g_pHead;

	g_iCounter++;
}

void   push_front(TStudent* pNewNode, TStudent* pLinkNode)
{
	_ASSERT(pLinkNode);
	_ASSERT(pLinkNode != g_pHead);
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

		g_iCounter++;
	}
}
TStudent* find(int iAge)
{
	for (TStudent* pNode = g_pHead->m_pNext;
		pNode != g_pTail;
		pNode = pNode->m_pNext)
	{
		if (pNode->m_iAge == iAge)
		{
			return pNode;
		}
	}
	return nullptr;
}
void Print(TStudent* pNode)
{
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%-6.2f\n",
		pNode->m_szName,
		pNode->m_iAge,
		pNode->m_iKor,
		pNode->m_iEng,
		pNode->m_iMat,
		pNode->m_iTotal,
		pNode->m_fAverage);
}
void Print(int index)
{
	TStudent* pFind = find(index);
	if (pFind == nullptr) return;
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%-6.2f\n",
		pFind->m_szName,
		pFind->m_iAge,
		pFind->m_iKor,
		pFind->m_iEng,
		pFind->m_iMat,
		pFind->m_iTotal,
		pFind->m_fAverage);
}
void PrintAll()
{
	for (TStudent* pNode = begin();
		pNode != end();
		pNode = next())
	{
		Print(pNode);
	}
	/*for (TStudent* pNode = rbegin();
		pNode != rend();
		pNode = rnext())
	{
		Print(pNode);
	}*/
}
TStudent* begin() {
	g_pCurNode = g_pHead->m_pNext;
	return g_pHead->m_pNext;
}
TStudent* end() { return g_pTail; }
TStudent* next() {
	TStudent* pTemp = g_pCurNode->m_pNext;
	g_pCurNode = pTemp;
	return pTemp;
}
TStudent* rbegin() {
	g_pCurNode = g_pTail->m_pPrev;
	return g_pTail->m_pPrev;
}
TStudent* rend() { return g_pHead; }
TStudent* rnext() {
	TStudent* pTemp = g_pCurNode->m_pPrev;
	g_pCurNode = pTemp;
	return pTemp;
}
void   erase(TStudent* pNode)
{
	_ASSERT(pNode);
	_ASSERT(pNode != g_pHead);
	_ASSERT(pNode != g_pTail);
	TStudent* pPrev = pNode->m_pPrev;
	TStudent* pNext = pNode->m_pNext;
	free(pNode);
	pNode = nullptr;
	pPrev->m_pNext = pNext;
	pNext->m_pPrev = pPrev;
}
void clear()
{
	TStudent* pNode = g_pHead->m_pNext;
	while (pNode != g_pTail)
	{
		TStudent* pNext = pNode->m_pNext;
		free(pNode);
		pNode = pNext;
	}
}
void    sort()
{
	// 1, 리스트에서 가장 작은 노드 검색한다.
	// 2, 맨앞에 추가한다. 기존 데이터 이동한다.
	// 3, (직전에 추가된 Next)반복한다.
	// 4, 추가노드의 이동이후에 pNext==g_pTail

	// 1, 빈파일 생성, 가장 작은 노드 -> 출력 -> 삭제
	// 2, 반복 (g_pHead->pNext== g_pTail)
	// 3, 파일 로드.

	// 1, 제2 연결리스트 생성
	// 2, 가장 작은 노드 -> 제2 연결리스트 추가 -> 삭제
	// 2, 반복 (g_pHead->pNext== g_pTail)
}