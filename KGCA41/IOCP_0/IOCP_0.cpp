
#include <windows.h>
#include <iostream>


DWORD g_dwMaxReadSize = 4096 * 4096;
DWORD g_dwMaxWriteSize = 4096 * 4096;
wchar_t* g_pFileBuffer = nullptr;

OVERLAPPED readOV = { 0, };
OVERLAPPED writeOV = { 0, };
LARGE_INTEGER g_LoadFileSize;
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
bool hWriteCopyFile(std::wstring file, std::wstring copyfile)
{
	HANDLE hReadFile = CreateFile(file.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	HANDLE hWriteFile = CreateFile(copyfile.c_str(),
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (hReadFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	if (hWriteFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}	

	::GetFileSizeEx(hReadFile, &g_LoadFileSize);
	if (g_dwMaxReadSize > g_LoadFileSize.QuadPart)
	{
		g_dwMaxReadSize = g_LoadFileSize.QuadPart;
	}

	// 버퍼의 용량의 잔여량이 입출력 단위보다 작으면 오류가 발생한다. 
	g_pFileBuffer = new wchar_t[g_LoadFileSize.QuadPart];

	DWORD dwReadOffset = g_dwMaxReadSize;
	DWORD dwWriteOffset = g_dwMaxWriteSize;
	bool bReadEnd = false;
	while (bReadEnd==false)
	{
		DWORD dwCurrent = FileRead(hReadFile, dwReadOffset);
		if (dwCurrent < g_dwMaxReadSize && g_LoadFileSize.QuadPart == g_LargerRead.QuadPart + dwCurrent)
		{
			bReadEnd = true;
		}
		else
		{
			g_LargerRead.QuadPart += dwCurrent;
			readOV.Offset = g_LargerRead.LowPart;
			readOV.OffsetHigh = g_LargerRead.HighPart;

			if ((readOV.Offset + g_dwMaxReadSize) > g_LoadFileSize.QuadPart)
			{
				dwReadOffset = (g_LoadFileSize.QuadPart - readOV.Offset);
			}
		}
		DWORD dwWriteCurrent = FileWrite(hWriteFile, dwWriteOffset);
		if (dwWriteCurrent < g_dwMaxWriteSize && 
			g_LoadFileSize.QuadPart == g_LargerWrite.QuadPart + dwWriteCurrent)
		{
			break;
		}
		g_LargerWrite.QuadPart += dwWriteCurrent;
		writeOV.Offset = g_LargerWrite.LowPart;
		writeOV.OffsetHigh = g_LargerWrite.HighPart;

		if ((writeOV.Offset + g_dwMaxWriteSize) > g_LoadFileSize.QuadPart)
		{
			dwWriteOffset = (g_LoadFileSize.QuadPart - writeOV.Offset);
		}		
	}
	CloseHandle(hReadFile);
	CloseHandle(hWriteFile);
	
	return true;
}

int main()
{
	std::wstring readfile = L"test.iso";
	std::wstring writefile = L"Copy.iso";
	hWriteCopyFile(readfile, writefile);
	std::cout << "Hello World!\n";
}