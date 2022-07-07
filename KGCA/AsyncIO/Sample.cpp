#include <windows.h>
#include <iostream>

DWORD SectorsPerCluster;
DWORD BytesPerSector;
DWORD NumberOfFreeClusters;
DWORD TotalNumberOfClusters;

void main()
{
	WCHAR lpRootPathName[] = L"c:\\";
	GetDiskFreeSpace(lpRootPathName, 
		&SectorsPerCluster,
		&BytesPerSector,
		&NumberOfFreeClusters,
		&TotalNumberOfClusters);

	setlocale(LC_ALL, "KOREAN");
	//{
	//	HANDLE hFile = CreateFile(L"test.txt",
	//		GENERIC_WRITE, 0, NULL,
	//		CREATE_ALWAYS,
	//		FILE_ATTRIBUTE_NORMAL,
	//		NULL); // ���ϻ���
	//	if (hFile != NULL)
	//	{
	//		WCHAR buffer[] = L"fdfdfafadsfafa";
	//		DWORD dwLength = sizeof(buffer);
	//		DWORD dwWritten;
	//		BOOL ret = WriteFile(hFile, buffer, dwLength,
	//			&dwWritten, NULL);
	//		if (ret == TRUE)
	//		{
	//			std::wcout << L"aaaaaa��¼���";
	//		}
	//	}
	//	CloseHandle(hFile);


	//	HANDLE hFileRead = CreateFile(L"test.txt",
	//		GENERIC_READ, 0, NULL,
	//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	//		NULL); // ���ϻ���

	//	if (hFileRead != NULL)
	//	{
	//		WCHAR buffer[512] = { 0, };
	//		DWORD dwLength = sizeof(buffer);
	//		DWORD dwRead;
	//		BOOL ret = ReadFile(hFileRead, buffer, dwLength,
	//			&dwRead, NULL);
	//		if (ret == TRUE)
	//		{
	//			std::wcout << buffer;
	//		}
	//	}
	//	CloseHandle(hFileRead);
	//}

	WCHAR* g_buffer = NULL;
	LARGE_INTEGER filesize;
	OVERLAPPED g_ReadOV = { 0, };
	OVERLAPPED g_WriteOV = { 0, };
	{
		HANDLE hFileAsync = CreateFile(L"DXSDK_Jun10.exe",
			GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED |
			FILE_FLAG_NO_BUFFERING,
			NULL); // ���ϻ���
		if (hFileAsync != INVALID_HANDLE_VALUE)
		{
			GetFileSizeEx(hFileAsync, &filesize);	
			DWORD dwSize = 0;
			if (filesize.QuadPart % BytesPerSector != 0)
			{
				dwSize = filesize.QuadPart / BytesPerSector;
				dwSize = (dwSize+1) * BytesPerSector;
			}
			g_buffer = new WCHAR[filesize.QuadPart];
			DWORD dwRead;
			DWORD dwWritten;
			BOOL ret = ReadFile(hFileAsync, g_buffer,
				dwSize,
				&dwRead, &g_ReadOV);
			BOOL bPanding = FALSE;
			if (ret == FALSE)
			{
				DWORD dwError = GetLastError();
				if (dwError ==  ERROR_IO_PENDING)
				{
					std::wcout << L"�ε� ��";		
					bPanding = TRUE;
				}
				else
				{
					std::wcout << L"�ε����";
					bPanding = FALSE;
				}
			}
			else
			{
				std::wcout << L"�ε�Ϸ�";
			}
			// �ε� ���� ���
			DWORD dwTrans=0;
			BOOL  bReturn;
			while (bPanding)
			{				
				bReturn = GetOverlappedResult(hFileAsync,
					&g_ReadOV, &dwTrans, FALSE);
				if (bReturn == TRUE)
				{
					std::wcout << L"�ε� �Ϸ�";
					bPanding = FALSE;
				}
				else
				{
					DWORD dwError = GetLastError();
					if (dwError == ERROR_IO_INCOMPLETE)
					{
						std::wcout << L".";
					}
					else
					{
						std::wcout << L"����";
						bPanding = FALSE;
					}
				}
				Sleep(1000);
			}
		}
		CloseHandle(hFileAsync);
		std::wcout << L"\n������� ";

		//delete [] g_buffer;
		HANDLE hFileWriteAsync = CreateFile(L"isoCopy.iso",
			GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | 
			FILE_FLAG_WRITE_THROUGH,
			NULL); // ���ϻ���
		if (hFileWriteAsync != NULL)
		{
			DWORD dwLength = filesize.QuadPart;
			DWORD dwWritten;
			BOOL ret = WriteFile(hFileWriteAsync, g_buffer, dwLength,
				&dwWritten, &g_WriteOV);
			BOOL bPanding = FALSE;
			if (ret == FALSE)
			{
				DWORD dwError = GetLastError();
				if (dwError == ERROR_IO_PENDING)
				{
					std::wcout << L"��� ��";
					bPanding = TRUE;
				}
				else
				{
					std::wcout << L"��½���";
				}
			}
			else
			{
				std::wcout << L"��¿Ϸ�";
				bPanding = FALSE;
			}
			// �ε� ���� ���
			DWORD dwTrans = 0;
			BOOL  bReturn;
			while (bPanding)
			{				
				bReturn = GetOverlappedResult(hFileWriteAsync,
					&g_WriteOV, &dwTrans, FALSE);
				if (bReturn == TRUE)
				{
					std::wcout << L"��¿Ϸ�";
					bPanding = FALSE;
				}
				else
				{
					DWORD dwError = GetLastError();
					if (dwError == ERROR_IO_INCOMPLETE)
					{
						std::wcout << L".";
					}
					else
					{
						std::wcout << L"����";
						bPanding = FALSE;
					}
				}
				Sleep(1000);
			}
		}
		CloseHandle(hFileWriteAsync);
	}

}