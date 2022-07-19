#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
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
	friend std::ostream& operator << (std::ostream& os, TStudent& list);
	friend std::istream& operator >> (std::istream& is, TStudent& list);

	void Print()
	{
		std::cout << std::setw(8) << std::left
			<< m_szName << std::setw(8) << std::left
			<< m_iAge << std::setw(8) << std::left
			<< m_iKor << std::setw(8) << std::left
			<< m_iEng << std::setw(8) << std::left
			<< m_iMat << std::setw(8) << std::left
			<< m_iTotal << std::setw(8) << std::left
			<< m_fAverage 
			<< std::endl;
	}
public:
	TStudent();
	~TStudent();
};

