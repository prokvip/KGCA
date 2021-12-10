#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream> // std::cout, std::in
#include <memory.h>
#include <stdlib.h>
class TStudent
{
public:
	int			m_iIndex;
	int			m_iKor;
	int			m_iEng;
	int			m_iMat;
	int			m_iTotal;
	float		m_fAverage;

	TStudent*	m_pNext;
	int& operator [] (int id);
	// p1 * 3 , 3 * p1
	friend std::ostream& operator << (
		std::ostream& os,
		TStudent& data);
	TStudent()
	{
		m_iIndex = 0;
		m_iKor = 0;
		m_pNext = NULL;
	}
	TStudent(int i, int kor) 
	{
		m_iIndex = i;
		m_iKor = kor;
		m_pNext = NULL;
	}
	~TStudent()
	{
	}
};

