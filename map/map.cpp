// map.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <map>
class THuman
{
public :
    int m_iAge;+
    std::string m_szAdd;
    std::string n_szTel;
public:
    void Set(int, std::string, std::string);
};
void THuman::Set(int age, std::string add, std::string tel)
{
    m_iAge = age;
    m_szAdd = add;
    n_szTel = tel;
}
int main()
{
    std::map<std::string, THuman> map;// 키,값 한쌍구축된 자료구조
    THuman h1, h2, h3, h4;
    h1.Set(10, "서울", "010");
    h2.Set(20, "수산", "010");
    h3.Set(30, "부산", "010");
    h4.Set(40, "대전", "010");
    map["홍길동"] = h1;
    map["박개동"] = h2;    
    map.insert(std::make_pair("이기자", h3));
    map.insert(std::make_pair("서리발", h4));
    std::map<std::string, THuman>::iterator iter = map.find("이기자");
    
    if (iter != map.end())
    {
        std::string iKey    = iter->first;
        THuman& data  = iter->second;
        data.m_iAge = 99;
        std::cout << iKey << "";
        std::cout << data.m_iAge << "Hello World!\n";
        map.erase(iter);
    }
    map.clear();
    std::cout << "Hello World!\n";

    std::map<std::string, THuman*> map2;// 키,값 한쌍구축된 자료구조
    THuman* p1 = new THuman;
    THuman* p2 = new THuman;
    THuman* p3 = new THuman;
    THuman* p4 = new THuman;
    p1->Set(10, "서울", "010");
    p2->Set(20, "수산", "010");
    p3->Set(30, "부산", "010");
    p4->Set(40, "대전", "010");
    map2["홍길동"] = p1;
    map2["박개동"] = p2;
    map2.insert(std::make_pair("이기자", p3));
    map2.insert(std::make_pair("서리발", p4));
    auto iter2 = map2.find("이기자");

    if (iter2 != map2.end())
    {
        std::string iKey = iter2->first;
        THuman* data = iter2->second;
        data->m_iAge = 99;
        std::cout << iKey << "";
        std::cout << data->m_iAge << "Hello World!\n";
        delete data;
        data = nullptr;
        map2.erase(iter2);
    }
    for (auto it = map2.begin();
        it != map2.end();
        it++)
    {
        THuman* data = it->second;
        delete data;
        it->second = nullptr;
    }
    map2.clear();
    std::cout << "Hello World!\n";
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
