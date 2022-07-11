#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> // malloc
#include <string.h> // memset
#include <time.h>
#include <conio.h> //_getche
int g_iCounter = 0;
struct TStudent
{
	char		m_szName[4]; 
	int			m_iAge;
	int			m_iKor;
	int			m_iEng;
	int			m_iMat;
	int			m_iTotal;
	float		m_fAverage;	
	TStudent*	m_pNext;
};
TStudent* g_pHead = NULL;
TStudent* g_pTail = NULL;
TStudent* g_pNext = NULL;

void   Init();
void   AllPrint();
void   Print(TStudent* pNode);
void   AllFree();
void   Release();
void   InitData(int iCounter);
void   InsertBack(TStudent* pNewNode, TStudent* pLinkNode=NULL);
void   InsertFront(TStudent* pNewNode, TStudent* pLinkNode=NULL);
TStudent* Find(int iAge);
TStudent* NewNode();

void main()
{	
	//srand(time(NULL));
	Init();
	int iWork = 1;
	int iWorkType = 0;
	while (iWork)
	{
		_getche();
		switch (iWorkType)
		{
			case 0:
			{
				InitData(5);
				iWorkType = 1;
			}break;
			case 1:
			{
				AllPrint();
			}break;
			case 2:
			{
				TStudent* pFind = Find(7);
				Print(pFind);
			}break;
		}
	}
	
	AllFree();
	Release();	
}
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
		InsertBack(NewNode());
	}
	for (int iNode = iCounter; iNode < iCounter*2; iNode++)
	{		
		InsertFront(NewNode());
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

	g_pNext = g_pHead;
}
// pLinkNode != NULL 일때는 pLinkNode->m_pNext= pNewNode 삽입된다.
// pLinkNode == NULL 일때는 g_pNext->m_pNext = pNewNode 삽입된다.
void   InsertBack(TStudent* pNewNode, TStudent* pLinkNode)
{	
	if (pLinkNode == NULL)
	{
		g_pNext->m_pNext = pNewNode;
		pNewNode->m_pNext = g_pTail;
		g_pNext = pNewNode;		
	}
	else
	{
		TStudent* pNext = pLinkNode->m_pNext;
		pLinkNode->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;				
	}
	g_iCounter++;
}
// pLinkNode != NULL 일때는 pLinkNode->m_pNext= pNewNode 삽입된다.
// pLinkNode == NULL 일때는 g_pHead->m_pNext = pNewNode 삽입된다.
void   InsertFront(TStudent* pNewNode, TStudent* pLinkNode)
{
	if (pLinkNode == NULL)
	{
		TStudent* pNext = g_pHead->m_pNext;
		g_pHead->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;
	}
	else
	{
		TStudent* pNext = pLinkNode->m_pNext;
		pLinkNode->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;
	}
	g_iCounter++;
}
TStudent* Find(int iAge)
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
	return NULL;
}
void Print(TStudent* pNode)
{
	printf("%s %d %d %d %d %d %10.4f\n",
		pNode->m_szName,
		pNode->m_iAge,
		pNode->m_iKor,
		pNode->m_iEng,
		pNode->m_iMat,
		pNode->m_iTotal,
		pNode->m_fAverage);
}
void AllPrint()
{
	printf("총 %d명\n", g_iCounter);

	for (TStudent* pNode = g_pHead->m_pNext;
		pNode != g_pTail;
		pNode = pNode->m_pNext)
	{
		Print(pNode);
	}
}
void AllFree()
{
	TStudent* pNode = g_pHead->m_pNext;
	while (pNode != g_pTail)
	{
		TStudent* pNext = pNode->m_pNext;
		free(pNode);
		pNode = pNext;
	}
}