#pragma once
#include "TLinkedlist.h"
#include "TStudent.h"
#include "TFileIO.h"

//printf("\n출력(0), 저장(1), 로드(2), 초기값(3), 종료(9)");
enum work { Print = 0, Save, Load, Create, Find, Exit };

class TStudentManager
{
	TLinkedlist   m_LinkedList;
	static int    m_iTotalCounter;
public:
	bool    Init();
	void    SaveFile(const char* filename)const;
	void    LoadFile(const char* filename);
	bool    Run();
	void    Release();
	TStudent* NewStudent();
};

