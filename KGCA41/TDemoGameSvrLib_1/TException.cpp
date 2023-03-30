#include "TException.h"
#include <fstream>
#include <DbgHelp.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <strsafe.h>


#define MAX_BUF_SIZE 255

void MakeDump(EXCEPTION_POINTERS* e)
{
	TCHAR tszFileName[MAX_BUF_SIZE] = { 0 };
	SYSTEMTIME stTime = { 0 };
	GetSystemTime(&stTime);
	StringCbPrintf(tszFileName,
		_countof(tszFileName),
		_T("%s_%4d%02d%02d_%02d%02d%02d.dmp"),
		_T("SkylarkDump"),
		stTime.wYear,
		stTime.wMonth,
		stTime.wDay,
		stTime.wHour,
		stTime.wMinute,
		stTime.wSecond);

	HANDLE hFile = CreateFile(tszFileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = e;
	exceptionInfo.ClientPointers = FALSE;

	MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory | MiniDumpWithFullMemory),
		e ? &exceptionInfo : NULL,
		NULL,
		NULL);

	if (hFile)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}

}


LONG WINAPI exceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;

	THREADENTRY32 te32;
	DWORD myThreadId = GetCurrentThreadId();
	DWORD myProcessId = GetCurrentProcessId();

	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap != INVALID_HANDLE_VALUE)
	{
		te32.dwSize = sizeof(THREADENTRY32);

		if (Thread32First(hThreadSnap, &te32))
		{
			do
			{
				// stop thread(except this one)
				if (te32.th32OwnerProcessID == myProcessId && te32.th32ThreadID != myThreadId)
				{
					HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
					if (hThread)
					{
						SuspendThread(hThread);
					}
				}

			} while (Thread32Next(hThreadSnap, &te32));

		}

		CloseHandle(hThreadSnap);
	}

	// dump
	MakeDump(exceptionInfo);

	ExitProcess(1);

	// end
	return EXCEPTION_EXECUTE_HANDLER;

}