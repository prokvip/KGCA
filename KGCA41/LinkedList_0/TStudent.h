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
	TStudent*	m_pNext;
	TStudent*	m_pPrev;	
public:
	TStudent();
	~TStudent();
};

