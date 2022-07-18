#pragma once
#include "TLinkedList.h"
#include <iostream>
class TStudentManager
{
public:
	TLinkedList m_List;
public:
	void		InitData(int iCounter);
	void		AllPrint();
	void		Print(int index);
	void		Print(TStudent* pNode);
	TStudent*	NewNode();
};

