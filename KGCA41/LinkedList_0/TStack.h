#pragma once
#include "TLinkedList.h"
//class TStudentManager;
template<class T>
class TStack : private TLinkedList<T>
{
	//friend class TStudentManager;
private:
	T* begin();
	T* end();
	T* next();
	T* Find(int index);
public:
	void		clear();
	void		push(T* pNewNode);
	void		pop();
	T*	top();
private:
	int iValue=8;
public:
	TStack();
	~TStack();
};

