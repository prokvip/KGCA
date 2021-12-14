#pragma once

#include <iostream> 
class TFileIO
{
	FILE* m_pStream = nullptr;
public:
	bool     CreateFile(const  char* pFileName);
	bool     OpenFile(const  char* pFileName);
	void     CloseFile();
	size_t   Write(void const* pData,int iSize);
	size_t   Read(void* pData, int iSize);
};

