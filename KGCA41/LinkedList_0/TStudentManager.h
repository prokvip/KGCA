#pragma once
#include "TLinkedList.h"
#include <iostream>
class TStudentManager
{
public:
	TLinkedList m_List;
	friend std::ostream& operator << (std::ostream& os,  TStudentManager& mgr);
	friend std::istream& operator >> (std::istream& is,  TStudentManager& mgr);
public:
	void		InitData(int iCounter);
	void		AllPrint();
	void		Print(int index);
	void		Print(TStudent* pNode);
	TStudent*	NewNode();
};

