#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
class TStudent
{
public:
	int			iIndex;
	int			iKor;
	TStudent*	pNext;
	TStudent()
	{
		iIndex = 0;
		iKor = 0;
		pNext = NULL;
	}
	TStudent(int i, int kor) 
	{
		iIndex = i;
		iKor = kor;
		pNext = NULL;
	}
	~TStudent()
	{
	}
};

