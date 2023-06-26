#pragma once
#include "TStd.h"
#include "TItem.h"
class TItemManager
{
	TLinkedlist<TItem>   m_LinkedList;
	static int    m_iTotalCounter;
public:
	bool    Init();
	void    SaveFile(const char* filename)const;
	void    LoadFile(const char* filename);
	bool    Run();
	void    Release();
	TItem* NewStudent();
};

