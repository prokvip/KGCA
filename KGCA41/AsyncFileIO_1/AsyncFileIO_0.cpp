// AsyncFileIO_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <windows.h>
#include <iostream>
DWORD g_dwMaxReadSize = 4096 * 4096 * 100;
wchar_t* g_pFileBuffer = 0;

OVERLAPPED readOV = { 0, };
DWORD dwRead = 0;
LARGE_INTEGER FileSize;


DWORD dwReadCounter = 0;
LARGE_INTEGER g_LargerRead = { 0, };

DWORD FileRead( HANDLE hFile, DWORD dwOffset)
{   
    wchar_t* pOffsetData = 0;
    pOffsetData = &g_pFileBuffer[g_LargerRead.QuadPart];

    BOOL ret = ::ReadFile(hFile, pOffsetData, dwOffset,
                            &dwRead, &readOV);
    
    bool bPending = false;
    if (ret == FALSE)
    {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_IO_PENDING)
        {
            // 읽기 중.
            bPending = true;
        }
        else
        {
            int k = 0;
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
        else
        {
            if (dwRead != g_dwMaxReadSize)
            {
                int k = 0;
            }
        }
    }
    return dwRead;
}
DWORD AsyncLoad(std::wstring file)
{
    HANDLE hFile = CreateFile(file.c_str(),
        GENERIC_READ, 0, NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);   
    

    if (hFile != INVALID_HANDLE_VALUE)
    {
        ::GetFileSizeEx(hFile, &FileSize);
        if (g_dwMaxReadSize > FileSize.LowPart)
        {
            g_dwMaxReadSize = FileSize.LowPart;
        }
        // 버퍼의 용량의 잔여량이 입출력 단위보다 작으면 오류가 발생한다. 
        g_pFileBuffer = new wchar_t[FileSize.LowPart];
        dwReadCounter = FileSize.LowPart / g_dwMaxReadSize;
        
        DWORD dwOffset = g_dwMaxReadSize;
        while (1)
        {            
            DWORD dwCurrent = FileRead(hFile, dwOffset);
            if (dwCurrent < g_dwMaxReadSize ||
                FileSize.LowPart == dwCurrent)
            {
                break;
            }
            g_LargerRead.QuadPart += dwCurrent;
            readOV.Offset = g_LargerRead.LowPart;
            readOV.OffsetHigh = g_LargerRead.HighPart;

            if ((readOV.Offset+ g_dwMaxReadSize) > FileSize.LowPart)
            {
                dwOffset = (FileSize.LowPart - readOV.Offset);
            }
        }        
        CloseHandle(hFile);
    }
    return dwRead;
}

DWORD AsyncCopy(std::wstring file, DWORD dwFileSize)
{
    HANDLE hFile = CreateFile(file.c_str(),
        GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);


    OVERLAPPED whiteOV = { 0, };
    DWORD dwTrans = 0;
    LARGE_INTEGER FileSize;
    bool bPending = false;
    if (hFile != INVALID_HANDLE_VALUE)
    {
        BOOL ret = ::WriteFile(hFile, g_pFileBuffer,
            dwFileSize,
            &dwTrans, &whiteOV);
        if (ret == FALSE)
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                // 읽기 중.
                bPending = true;
            }
        }
        if (ret == TRUE)
        {
            // 로딩완료
        }
        while (bPending)
        {
            // 비동기 입출력의 결과를 확인하는 함수.
            ret = ::GetOverlappedResult(hFile, &whiteOV, &dwTrans, FALSE);
            if (ret == TRUE)
            {
                bPending = false;
            }
            std::cout << whiteOV.InternalHigh << " ";
        }
        CloseHandle(hFile);
    }
    return dwTrans;
}
int main()
{
    std::wstring readfile = L"test.exe";
    std::wstring writefile = L"Copy.iso";

    DWORD dwFileSize = AsyncLoad(readfile);
    //AsyncCopy(writefile, dwFileSize);
    std::cout << "Hello World!\n";
}
