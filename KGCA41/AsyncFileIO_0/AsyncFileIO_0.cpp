// AsyncFileIO_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <windows.h>
#include <iostream>
wchar_t* g_pFileBuffer = 0;
DWORD Load(std::wstring file)
{
    HANDLE hReadFile = CreateFile( file.c_str(),
                                    GENERIC_READ, 0, NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwRead = 0;
    LARGE_INTEGER FileSize;
    if (hReadFile != INVALID_HANDLE_VALUE)
    {
        ::GetFileSizeEx(hReadFile, &FileSize);
        g_pFileBuffer = new wchar_t[FileSize.LowPart];
        BOOL ret = ::ReadFile(hReadFile, g_pFileBuffer, 
                                FileSize.QuadPart,
                                 &dwRead, NULL);
        if (ret == TRUE)
        {

        }
        CloseHandle(hReadFile);
    }
   
    return dwRead;
}

DWORD Copy(std::wstring file, DWORD dwFileSize)
{
    HANDLE hWriteFile = CreateFile(file.c_str(),
        GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwWrite = 0;
    LARGE_INTEGER FileSize;
    if (hWriteFile != INVALID_HANDLE_VALUE)
    {  
        BOOL ret = ::WriteFile(hWriteFile, g_pFileBuffer,
            dwFileSize,
            &dwWrite, NULL);
        if (ret == TRUE)
        {

        }
        CloseHandle(hWriteFile);
    }

    return dwWrite;
}

DWORD AsyncLoad(std::wstring file)
{
    HANDLE hReadFile = CreateFile(file.c_str(),
        GENERIC_READ, 0, NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    
    
    OVERLAPPED readOV = { 0, };
    DWORD dwRead = 0;
    LARGE_INTEGER FileSize;
    bool bPending = false;
    if (hReadFile != INVALID_HANDLE_VALUE)
    {
        ::GetFileSizeEx(hReadFile, &FileSize);
        g_pFileBuffer = new wchar_t[FileSize.LowPart];
        BOOL ret = ::ReadFile(hReadFile, g_pFileBuffer,
            FileSize.QuadPart,
            &dwRead, &readOV);
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
            ret = ::GetOverlappedResult(hReadFile, &readOV, &dwRead, FALSE);
            if (ret == TRUE)
            {
                bPending = false;
            }
            std::cout << readOV.InternalHigh << " ";
        }
        CloseHandle(hReadFile);
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
    std::wstring readfile = L"Text.iso";
    std::wstring writefile = L"Copy.zip";
   // DWORD dwFileSize = Load(readfile);
   // Copy(writefile, dwFileSize);
    DWORD dwFileSize = AsyncLoad(readfile);
    AsyncCopy(writefile, dwFileSize);
    std::cout << "Hello World!\n";
}
