#include <iostream>
#include <string>
#include <tchar.h>
// 싱글바이트  영문, 문자, 한글 모두 1byte 표현
//std::string  == char // 멀티바이트코드
// 영문, 문자 1byte 표현, 한글 2byte
//  kgca홍길동
//std::wstring  == wchar // 유니코드
//  영문, 문자, 한글 2byte
//using string  = basic_string<char, char_traits<char>, allocator<char>>;
//using wstring = basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>>;
//typedef std::basic_string<char> C_STR;
using C_STR = std::basic_string<char>; // using string
using W_STR = std::basic_string<wchar_t>; // using wstring
using T_STR = std::basic_string<TCHAR>; // using string
static std::wstring mtw(std::string str)
{
    std::wstring ret = std::wstring(str.begin(), str.end());
    return  ret;
}
static std::string wtm(std::wstring str)
{
    return  std::string(str.begin(), str.end());
}

int main()
{
    char  name[10] = { 0, };
    C_STR name1 = "kgca1111";
    int iSize = name1.size();
    C_STR name2 = "game";
    if (name1 == name2){}
    name2 += "academy";
    const char* pName = name1.c_str();
    std::cout << pName << name1.c_str() <<"\n";

    wchar_t  name6[10] = { 0, };
    W_STR name3 = mtw(name1);
    iSize = name3.size();
    iSize = sizeof(wchar_t) * name3.size();
    W_STR name4 = L"game";
    if (name3 == name4) {}
    name4 += L"academy";
    wchar_t* pName2 = const_cast<wchar_t*>(name3.c_str());
    std::cout << pName2 << name4.c_str() << "\n";
}
