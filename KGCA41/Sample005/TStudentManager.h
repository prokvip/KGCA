#pragma once
#include "TStd.h"
#include "TStudent.h"

class TStudentManager
{
	TLinkedlist<TStudent>   m_LinkedList;
	static int    m_iTotalCounter;
public:
	bool    Init();
	void    SaveFile(const char* filename)const;
	void    LoadFile(const char* filename);
	bool    Run();
	void    Release();
	TStudent* NewStudent();
};

