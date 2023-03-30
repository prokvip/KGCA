#pragma once
#include <WinSock2.h>
LONG WINAPI exceptionFilter(EXCEPTION_POINTERS* exceptionInfo);
inline void crashAssert(bool isOk)
{
	if (isOk)
		return;
	int* crashVal = nullptr;
	*crashVal = 0xDEADBEEF;
}