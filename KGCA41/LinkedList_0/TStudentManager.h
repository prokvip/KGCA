#pragma once
#include "TStack.h"
class TStudentManager
{
public:
	TStack m_List;
public:
	void		InitData(int iCounter);
	void		AllPrint();
	void		Print(int index);
	void		Print(TStudent* pNode);
	TStudent*	NewNode();
};

