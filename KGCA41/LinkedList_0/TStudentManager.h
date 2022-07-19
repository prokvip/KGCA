#pragma once
#include "TStudent.h"
#include "TLinkedList.h"

class TStudentManager
{
public:
	TLinkedList<TItem> m_List;
	friend std::ostream& operator << (std::ostream& os,  TStudentManager& mgr);
	friend std::istream& operator >> (std::istream& is,  TStudentManager& mgr);
public:
	void		InitData(int iCounter);
	void		AllPrint();
	void		Print(int index);
	void		Print(TItem* pNode);
	TItem*	NewNode();
};

