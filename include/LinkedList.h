#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> // malloc
#include <string.h> // memset
#include <time.h>
#include <conio.h> //_getche
#include <crtdbg.h> // _ASSERT

struct TStudent
{
	char		m_szName[4];
	int			m_iAge;
	int			m_iKor;
	int			m_iEng;
	int			m_iMat;
	int			m_iTotal;
	float		m_fAverage;
	TStudent* m_pNext;
	TStudent* m_pPrev;
};

TStudent* begin();
TStudent* end();
TStudent* next();
TStudent* rbegin();
TStudent* rend();
TStudent* rnext();

void   Init();
void   PrintAll();
void   Print(int index);
void   Print(TStudent* pNode);
void   clear();
void   Release();
void   InitData(int iCounter);
void   push_back(TStudent* pNewNode);
void   push_front(TStudent* pNewNode);
void   push_back(TStudent* pNewNode, TStudent* pLinkNode);
void   push_front(TStudent* pNewNode, TStudent* pLinkNode);
void   erase(TStudent* pNode);
TStudent* find(int iAge);
TStudent* NewNode();
void    sort();
