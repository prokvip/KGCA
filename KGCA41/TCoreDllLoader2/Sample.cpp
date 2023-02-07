#include "TCoreDll.h"
#include <iostream>

typedef int (CALLBACK* GETVALUE)();
typedef TCoreDll*(CALLBACK* NEWCORE)();
void main()
{
	HMODULE dll = LoadLibrary(L"TCoreDll.dll");
	{
		GETVALUE lpGetValue;
		lpGetValue = (GETVALUE)GetProcAddress(dll, "?GerValue@@YAHXZ");
		int iValue = lpGetValue();
		std::cout << iValue;
		//std::cout << g_iValue;

		NEWCORE procClass = (NEWCORE)GetProcAddress(dll, "NewCore");
		TCoreDll* core = procClass();
		std::cout << core->m_iValue << std::endl;;
	}
	FreeLibrary(dll);
}