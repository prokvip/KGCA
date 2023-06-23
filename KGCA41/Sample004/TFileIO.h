#pragma once
#include <iostream>
class TFileIO
{
	FILE*   m_pFP = nullptr;
public:
	FILE*   CreateFile(const char* filename);
	void    CloseFile();

};

