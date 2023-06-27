#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
class TFileIO
{
	FILE*   m_pFP = nullptr;
public:
	FILE*   CreateFile(const char* filename,
					   const char* fpMode = "w");
	void    CloseFile();
	void    Release();
};

