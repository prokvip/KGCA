#pragma once
#include "TStudentMiddle.h"
#include "TStudentHigh.h"
#include "TStudentCollege.h"
class TNodeBox
{
public:
	TStudent* m_pData;
	TStudent* NewData(int iIndex);
	friend std::ostream& operator << (
		std::ostream& os,
		TNodeBox& data);
public:
	TNodeBox*	m_pNext;
	TNodeBox();
	TNodeBox(int iIndex);
	~TNodeBox();
};

