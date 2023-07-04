#pragma once
#include "TStd.h"
#include "TStudent.h"

class TStudentManager
{
	std::vector<TStudent> list;
	
	TLinkedlist<TStudent>   m_LinkedList;//링크드 리스트
	TLinkedlist<TLinkedlist<TLinkedlist<TStudent>>>   m_LinkedListAll;
	static int    m_iTotalCounter;
public:
	bool    Init();
	// 파일 저장
	void    SaveFile(const char* filename)const;
	void    LoadFile(const char* filename);
	bool    Run();
	void    Release();
	TStudent* NewStudent();
};

