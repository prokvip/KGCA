#pragma once
#include "TLinkedList.h"
//class TStudentManager;

class TStack : private TLinkedList
{
	//friend class TStudentManager;
private:
	TStudent* begin();
	TStudent* end();
	TStudent* next();
	TStudent* Find(int index);
public:
	void		clear();
	void		push(TStudent* pNewNode);
	void		pop();
	TStudent*	top();	
private:
	int iValue=8;
public:
	TStack();
	~TStack();
};

