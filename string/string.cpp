// string.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
int main()
{
    //  싱글바이트(1) : 1문자를 표현하는 메모리 바이트
    //  멀티바이트(1 or 2) : KGCA게임
    //  유니코드= Only 2 : KGCA게임
    //char data3[] = "KgCA";
    //char data4[] = "KGca";
    //int iRet= _stricmp(data3, data4);
    //if ( iRet  == 0)
    //{
    //    printf("\n");
    //}
    //char copy[50] = { 0, };
    //// 0 ~ 9, 11 ~ 49
    //int iLength = strlen(data3);
    //// 버퍼오버런 문제를 해결하기 위해서.
    //// strcpy -> strcpy_s
    //int iSize = _countof(copy);
    //strcpy_s(copy, _countof(copy), data3);
    //strcpy_s(&copy[iLength], _countof(copy), data4);
    //printf("\n %s", copy);

    std::string data1 = "KGCA";    
    std::string data2 = "게임아카";
    std::string sum = data1 + data2;
    int iLen = sum.size();
    if (data1 <= data2)
    {
        std::cout << sum << "Hello World!\n";
    }
    sum.clear();
    std::cout << sum << "Hello World!\n";


    std::wstring d1 = L"KGCA";
    std::wstring d2 = L"게임아카";
    std::wstring d3 = d1 + d2;

    iLen = d3.size();
    if (d1 <= d2)
    {
        std::wcout << d3 << "Hello World!\n";
    }
    d3.clear();
    std::wcout << d3 << "Hello World!\n";
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
