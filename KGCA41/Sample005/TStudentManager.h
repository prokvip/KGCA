#pragma once
#include "TStd.h"
#include "TStudent.h"

class TStudentManager
{
	std::vector<TStudent> list;
	
	TLinkedlist<TStudent>   m_LinkedList;//��ũ�� ����Ʈ
	TLinkedlist<TLinkedlist<TLinkedlist<TStudent>>>   m_LinkedListAll;
	static int    m_iTotalCounter;
public:
	bool    Init();
	// ���� ����
	void    SaveFile(const char* filename)const;
	void    LoadFile(const char* filename);
	bool    Run();
	void    Release();
	TStudent* NewStudent();
};
