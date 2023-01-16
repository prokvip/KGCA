
#include <windows.h>
#include <iostream>
#include <process.h>//_beginthreadex

HANDLE  g_hIOCP;
HANDLE  g_hEventFinish;
HANDLE hReadFile;
HANDLE hWriteFile;
const DWORD g_dwMaxReadSize = 4096*4096;
const DWORD g_dwMaxWriteSize = 4096 * 4096;
wchar_t* g_pFileBuffer=nullptr;

OVERLAPPED readOV = { 0, };
OVERLAPPED writeOV = { 0, };
LARGE_INTEGER g_LoadFileSize;
LARGE_INTEGER g_LargerRead = { 0, };
LARGE_INTEGER g_LargerWrite = { 0, };

unsigned WINAPI WorkProc(LPVOID arg)
{	
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOV;

	DWORD dwReadOffset = g_dwMaxReadSize;
	DWORD dwWriteOffset = g_dwMaxReadSize;
	while (1)
	{
		DWORD dwEvent = WaitForSingleObject(g_hEventFinish, 0);
		if( dwEvent == WAIT_OBJECT_0)
		{
			break;
		}
		// 비동기 읽기 완성 여부 판단
		BOOL bRet = ::GetQueuedCompletionStatus(g_hIOCP, 
			&dwTransfer, 
			&KeyValue, 
			&pOV, INFINITE);
		
		if (bRet== TRUE)
		{
			// 읽기 또는 쓰기 비동기 완성
			if (KeyValue == 1000)
			{
				g_LargerRead.QuadPart += dwTransfer;
				readOV.Offset = g_LargerRead.LowPart;
				readOV.OffsetHigh = g_LargerRead.HighPart;

				DWORD dwTrans = 0;
				wchar_t* pOffsetData = &g_pFileBuffer[g_LargerWrite.QuadPart];
				BOOL ret = ::WriteFile( hWriteFile, pOffsetData, 
					dwTransfer, &dwTrans, &writeOV);

			}
			if (KeyValue == 2000)
			{
				if (readOV.Offset == g_LoadFileSize.QuadPart)
				{
					::SetEvent(g_hEventFinish);
				}
				else
				{

					g_LargerWrite.QuadPart += dwTransfer;
					writeOV.Offset = g_LargerWrite.LowPart;
					writeOV.OffsetHigh = g_LargerWrite.HighPart;

					wchar_t* pOffsetData = &g_pFileBuffer[g_LargerRead.QuadPart];


					DWORD dwRead;
					BOOL ret = ::ReadFile(hReadFile, pOffsetData,
						dwReadOffset, &dwRead, &readOV);
				}
			}
		}	
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_HANDLE_EOF)
			{
				break;
			}
			else
			{
				int k = 0;
			}
		}
	}
	return 0;
}

void main()
{
	g_hEventFinish = ::CreateEvent(0, TRUE, FALSE, 0);

	std::wstring readfile = L"test.iso";
	std::wstring writefile = L"Copy.iso";
	hReadFile = CreateFile(readfile.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hReadFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	hWriteFile = CreateFile(writefile.c_str(),
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hWriteFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//1) 
	g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0,0,0);
	::CreateIoCompletionPort(hReadFile, g_hIOCP, 1000, 0);
	::CreateIoCompletionPort(hWriteFile, g_hIOCP, 2000, 0);



	
	::GetFileSizeEx(hReadFile, &g_LoadFileSize);
	// 버퍼의 용량의 잔여량이 입출력 단위보다 작으면 오류가 발생한다. 
	g_pFileBuffer = new wchar_t[g_LoadFileSize.QuadPart];

	unsigned int idRead;
	unsigned long hWorkThread =
		_beginthreadex(NULL, 0, WorkProc, (void*)readfile.c_str(), 0, &idRead);

	g_LargerRead.QuadPart += 0;
	readOV.Offset = g_LargerRead.LowPart;
	readOV.OffsetHigh = g_LargerRead.HighPart;

	DWORD dwRead;
	BOOL ret = ::ReadFile(   hReadFile, g_pFileBuffer,
							 g_dwMaxReadSize, &dwRead, &readOV);


	/*while (1)
	{
		Sleep(100);
		float fCurentSize = (float)g_LargerRead.QuadPart / (float)g_LoadFileSize.QuadPart;
		std::cout << fCurentSize * 100.0f << "\n";
		if (g_LargerRead.QuadPart == g_LoadFileSize.QuadPart)
		{
			break;
		}
	}*/

	WaitForSingleObject((HANDLE)hWorkThread, INFINITE);
	CloseHandle((HANDLE)hWorkThread);


	CloseHandle(hReadFile);
	CloseHandle(hWriteFile);

	delete g_pFileBuffer;
}
