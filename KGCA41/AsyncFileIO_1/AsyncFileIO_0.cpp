
#include <windows.h>
#include <iostream>


DWORD g_dwMaxReadSize = 4096 * 4096;
DWORD g_dwMaxWriteSize = 4096 * 4096;
wchar_t* g_pFileBuffer = nullptr;

OVERLAPPED readOV = { 0, };
OVERLAPPED writeOV = { 0, };
LARGE_INTEGER g_LoadFileSize;
DWORD dwReadCounter = 0;
DWORD dwWriteCounter = 0;
LARGE_INTEGER g_LargerRead = { 0, };
LARGE_INTEGER g_LargerWrite = { 0, };

DWORD FileRead(HANDLE hFile, DWORD dwOffset)
{
	DWORD dwRead = 0;
	wchar_t* pOffsetData = &g_pFileBuffer[g_LargerRead.QuadPart];
	BOOL ret = ::ReadFile(hFile, pOffsetData, dwOffset, &dwRead, &readOV);

	bool bPending = false;
	if (ret == FALSE)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_IO_PENDING)
		{
			// 읽기 중.
			bPending = true;
		}
	}
	if (ret == TRUE)
	{
		std::cout << "로딩완료!\n";
	}

	while (bPending)
	{
		// 비동기 입출력의 결과를 확인하는 함수.
		ret = ::GetOverlappedResult(hFile, &readOV, &dwRead, FALSE);
		if (ret == TRUE)
		{
			if (dwRead != g_dwMaxReadSize)
			{
				bPending = false;
			}
			bPending = false;
			std::cout << dwReadCounter-- << std::endl;
		}
	}
	return dwRead;
}
DWORD FileWrite(HANDLE hFile, DWORD dwOffset)
{
	DWORD dwTrans = 0;
	wchar_t* pOffsetData = &g_pFileBuffer[g_LargerWrite.QuadPart];
	BOOL ret = ::WriteFile(hFile, pOffsetData, dwOffset, &dwTrans, &writeOV);

	bool bPending = false;
	if (ret == FALSE)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_IO_PENDING)
		{
			// 작업 중.
			bPending = true;
		}
	}
	if (ret == TRUE)
	{
		std::cout << "완료!\n";
	}

	while (bPending)
	{
		// 비동기 입출력의 결과를 확인하는 함수.
		ret = ::GetOverlappedResult(hFile, &writeOV, &dwTrans, FALSE);
		if (ret == TRUE)
		{
			if (dwTrans != g_dwMaxWriteSize)
			{
				// error
			}
			bPending = false;
			std::cout << dwWriteCounter-- << std::endl;
		}
	}
	return dwTrans;
}
LARGE_INTEGER AsyncLoad(std::wstring file)
{
	HANDLE hFile = CreateFile(file.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);


	if (hFile != INVALID_HANDLE_VALUE)
	{
		::GetFileSizeEx(hFile, &g_LoadFileSize);
		if (g_dwMaxReadSize > g_LoadFileSize.QuadPart)
		{
			g_dwMaxReadSize = g_LoadFileSize.QuadPart;
		}
		// 버퍼의 용량의 잔여량이 입출력 단위보다 작으면 오류가 발생한다. 
		g_pFileBuffer = new wchar_t[g_LoadFileSize.QuadPart];
		dwReadCounter = g_LoadFileSize.LowPart / g_dwMaxReadSize;

		DWORD dwOffset = g_dwMaxReadSize;
		while (1)
		{
			DWORD dwCurrent = FileRead(hFile, dwOffset);
			if (dwCurrent < g_dwMaxReadSize && g_LoadFileSize.QuadPart == g_LargerRead.QuadPart + dwCurrent)
			{
				break;
			}
			g_LargerRead.QuadPart += dwCurrent;
			readOV.Offset = g_LargerRead.LowPart;
			readOV.OffsetHigh = g_LargerRead.HighPart;

			if ((readOV.Offset + g_dwMaxReadSize) > g_LoadFileSize.QuadPart)
			{
				dwOffset = (g_LoadFileSize.QuadPart - readOV.Offset);
			}
		}
		CloseHandle(hFile);
	}
	return g_LoadFileSize;
}

LARGE_INTEGER AsyncCopy(std::wstring file, LARGE_INTEGER FileSize)
{
	HANDLE hFile = CreateFile(file.c_str(),
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	DWORD dwTrans = 0;
	bool bPending = false;

	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwWriteCounter = FileSize.LowPart / g_dwMaxWriteSize;
		DWORD dwOffset = g_dwMaxWriteSize;
		while (1)
		{
			DWORD dwCurrent = FileWrite(hFile, dwOffset);
			if (dwCurrent < g_dwMaxWriteSize && FileSize.QuadPart == g_LargerWrite.QuadPart + dwCurrent)
			{
				break;
			}
			g_LargerWrite.QuadPart += dwCurrent;
			writeOV.Offset = g_LargerWrite.LowPart;
			writeOV.OffsetHigh = g_LargerWrite.HighPart;

			if ((writeOV.Offset + g_dwMaxWriteSize) > FileSize.QuadPart)
			{
				dwOffset = (FileSize.QuadPart - writeOV.Offset);
			}
		}
		CloseHandle(hFile);
	}
	return FileSize;
}

int main()
{
	std::wstring readfile = L"test.exe";
	std::wstring writefile = L"Copy.iso";

	LARGE_INTEGER g_LoadFileSize = AsyncLoad(readfile);
	AsyncCopy(writefile, g_LoadFileSize);
	std::cout << "Hello World!\n";
}