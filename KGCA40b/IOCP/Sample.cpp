#include <windows.h>
#include <iostream>
#define BlockSize (104857600)	
#define MAX_WORKER_THREAD 3
WCHAR* g_buffer = NULL;
LARGE_INTEGER filesize;
DWORD	   g_dwCurrentRead = 0;
DWORD	   g_dwCurrentWrite = 0;
OVERLAPPED g_ReadOVArray[100];
OVERLAPPED g_WriteOVArray[100];


HANDLE g_hFileRead;
HANDLE g_hFileWrite;
HANDLE g_hWorkThread[MAX_WORKER_THREAD];
/*완료포트 관리 객체*/
HANDLE g_hIOCP;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;
HANDLE g_hKillEvent;
bool DispatchRead(DWORD dwTransfer, OVERLAPPED* ov)
{
	g_LargeRead.QuadPart += dwTransfer;

	++g_dwCurrentWrite;
	g_WriteOVArray[g_dwCurrentWrite].Offset
		= g_LargeWrite.LowPart;
	g_WriteOVArray[g_dwCurrentWrite].OffsetHigh
		= g_LargeWrite.HighPart;

	DWORD dwWritten;
	BOOL ret = WriteFile(g_hFileWrite,
		g_buffer,
		dwTransfer,
		&dwWritten,
		&g_WriteOVArray[g_dwCurrentWrite]);
	if (ret == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool DispatchWrite(DWORD dwTransfer, OVERLAPPED* ov)
{
	g_LargeWrite.QuadPart += dwTransfer;

	if (filesize.QuadPart == g_LargeWrite.QuadPart)
	{
		::SetEvent(g_hKillEvent);
		return true;
	}

	++g_dwCurrentRead;
	g_ReadOVArray[g_dwCurrentRead].Offset
		= g_LargeRead.LowPart;
	g_ReadOVArray[g_dwCurrentRead].OffsetHigh
		= g_LargeRead.HighPart;

	DWORD dwRead;
	BOOL ret = ReadFile(g_hFileRead,
		g_buffer,
		BlockSize,
		&dwRead,
		&g_ReadOVArray[g_dwCurrentRead]);
	if (ret == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
DWORD WINAPI WorkerThread(LPVOID param)
{
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		// 완료 큐에 데이터가 있으면 작업시작
		BOOL bReturn = ::GetQueuedCompletionStatus(
			g_hIOCP,
			&dwTransfer,
			&KeyValue,
			&pOverlapped, 1);

		if (bReturn == TRUE)
		{
			if (KeyValue == 1111)
			{
				if (!DispatchRead(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
			if (KeyValue == 2222)
			{
				if (!DispatchWrite(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
		}
		else
		{
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(g_hKillEvent);
				break;
			}
		}
	}
	return 1;
}
void main()
{
	setlocale(LC_ALL, "KOREAN");
	g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	g_hFileRead = CreateFile(L"DXSDK_Jun10.exe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL); // 파일생성
	if (g_hFileRead == INVALID_HANDLE_VALUE)
	{
		return;
	}
	g_hFileWrite = CreateFile(L"copy.exe", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL); // 파일생성
	if (g_hFileWrite == INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hFileRead);
		return;
	}

	// 비동기입출력 작업결과 저장 큐
	g_hIOCP = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE, 0, 0, 0);
	::CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1111, 0);
	::CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 2222, 0);


	GetFileSizeEx(g_hFileRead, &filesize);

	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	g_buffer = new WCHAR[BlockSize];

	for (int iThread=0;
		 iThread < MAX_WORKER_THREAD;  iThread++)
	{
		DWORD id;
		g_hWorkThread[iThread]=::CreateThread(0, 0, WorkerThread,
			nullptr, 0, &id);
	}
	g_LargeRead.QuadPart = 0;
	g_LargeWrite.QuadPart = 0;
	// 비동기 로드	
	BOOL ret = ReadFile(g_hFileRead, g_buffer, 
		BlockSize, &dwRead, 
		&g_ReadOVArray[g_dwCurrentRead++]);
	
	WaitForMultipleObjects(MAX_WORKER_THREAD,
		g_hWorkThread, TRUE, INFINITE);
	
	for (int iThread = 0;
		iThread < MAX_WORKER_THREAD;  iThread++)
	{
		CloseHandle(g_hWorkThread[iThread]);
	}
	CloseHandle(g_hKillEvent);
	CloseHandle(g_hIOCP);
	CloseHandle(g_hFileRead);
	CloseHandle(g_hFileWrite);
	return;
}