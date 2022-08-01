// vector_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector> // 배열
#include <algorithm> // 배열

bool compare(int a, int b)
{
    return a == b;
}
int main()
{
    std::vector<int>  list;
    std::vector<int>  findlist;
    findlist.push_back(17);

    for (int i = 0; i < 20; i++)
    {
        list.push_back(i);
    }
    for (int i = 0; i < 5; i++)
    {
        list.insert(list.begin(), 10+i);
    }
    for (int i = 0; i < 5; i++)
    {
        list.insert(list.end(), 20 + i);
    }   
    std::sort(list.begin(), list.end());
    std::vector<int>::iterator iter = 
        std::find(list.begin(), list.end(), 17);
    list.erase(iter);

    int iValue1 = list.front();
    int iValue2 = list.back();
    list.pop_back();
    
    for (std::vector<int>::iterator iter = list.begin();
        iter != list.end();
        iter++)
    {
        //int* iValue = iter._Ptr; // *iter        
        std::cout << " " << *iter;
    }
    std::cout << std::endl;
    for (int i = 0; i < list.size(); i++)
    {
        std::cout << " " << list[i];
    }
    std::cout << std::endl;
    for (int value : list) // 복사
    {
        value = 100;
        std::cout << " " << value;
    }
    std::cout << std::endl;
    for (int& value : list)
    {
        value = 100;
        std::cout << " " << value;
    }
    list.clear();
    std::cout << "Hello World!\n";
}
