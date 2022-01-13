#include <windows.h>
#include <iostream>
#define BlockSize 2048	
void main()
{
	setlocale(LC_ALL, "KOREAN");
	WCHAR* g_buffer = NULL;
	LARGE_INTEGER filesize;
	OVERLAPPED g_ReadOV = { 0, };
	OVERLAPPED g_WriteOV = { 0, };
	HANDLE hFileAsync = CreateFile(L"DXSDK_Jun10.exe",	GENERIC_READ | GENERIC_WRITE, 0, NULL,	OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	NULL); // 파일생성
	if (hFileAsync == INVALID_HANDLE_VALUE)
	{
		return;
	}
	HANDLE hFileCopy = CreateFile(L"copy.exe",	GENERIC_WRITE, 0, NULL,	CREATE_ALWAYS,	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	NULL); // 파일생성
	if (hFileCopy == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileAsync);
		return;
	}
	GetFileSizeEx(hFileAsync, &filesize);
	
	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	g_buffer = new WCHAR[BlockSize];
	for( int i=0; ; i++)
	{
		ZeroMemory(g_buffer, BlockSize);
		// 비동기 로드
		g_ReadOV.Offset = i* BlockSize;
		g_ReadOV.OffsetHigh  = 0;
		BOOL ret = ReadFile(hFileAsync, g_buffer,BlockSize,&dwRead, &g_ReadOV);
		BOOL bReturn = GetOverlappedResult(hFileAsync,&g_ReadOV, &dwRead, TRUE);
		
		// 비동기 출력
		g_WriteOV.Offset = i * BlockSize;
		g_WriteOV.OffsetHigh = 0;
		DWORD dwWritten;
		ret = WriteFile(hFileCopy, g_buffer, dwRead,&dwWritten, &g_WriteOV);
		bReturn = GetOverlappedResult(hFileCopy,&g_WriteOV, &dwWritten, TRUE);
		dwTotalWrite += dwWritten;

		if (filesize.LowPart == dwTotalWrite)
		{
			break;
		}
	}	
	CloseHandle(hFileAsync);
	CloseHandle(hFileCopy);
}