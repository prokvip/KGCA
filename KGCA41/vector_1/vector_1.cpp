// vector_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector> // 배열
#include <algorithm> // 배열

class TItem
{
public:
    int iValue;    
    bool operator == (const TItem& p)
    {
        return iValue == p.iValue;
    }
  
    TItem(int i) : iValue(i)
    {

    }
};
struct isValue
{
    int m_value;
    isValue(int value) : m_value(value) {}
    bool operator()(const TItem* cls) 
    {
        return (cls->iValue == m_value);
    }
};
bool  compare(TItem* a, TItem* b)
{
    return a->iValue < b->iValue;
}
int g_iFindValue;
bool  find_TItem(TItem* a)
{
    return a->iValue == g_iFindValue;
}
int main()
{
    /*std::vector<TItem>  list2;
    list2.push_back(17);
    TItem finditem3(17);
    std::vector<TItem>::iterator iter3 =
        std::find(list2.begin(), list2.end(), finditem3);*/

    std::vector<TItem*>  list;  
    for (int i = 0; i < 20; i++)
    {
        list.push_back(new TItem(i));
    }
    for (int i = 0; i < 5; i++)
    {
        list.insert(list.begin(), new TItem(10+i));
    }
    for (int i = 0; i < 5; i++)
    {
        list.insert(list.end(), new TItem(20 + i));
    }
    std::sort(list.begin(), list.end(), compare);
    g_iFindValue = 17;

    // find_if -> operator ()
    std::vector<TItem*>::iterator iter = std::find_if(list.begin(), list.end(), isValue(17));
    list.erase(iter);

    //int iValue1 = list.front();
    //int iValue2 = list.back();
    //list.pop_back();

    //for (std::vector<int>::iterator iter = list.begin();
    //    iter != list.end();
    //    iter++)
    //{
    //    //int* iValue = iter._Ptr; // *iter        
    //    std::cout << " " << *iter;
    //}
    //std::cout << std::endl;
    //for (int i = 0; i < list.size(); i++)
    //{
    //    std::cout << " " << list[i];
    //}
    //std::cout << std::endl;
    //for (int value : list) // 복사
    //{
    //    value = 100;
    //    std::cout << " " << value;
    //}
    //std::cout << std::endl;
    //for (int& value : list)
    //{
    //    value = 100;
    //    std::cout << " " << value;
    //}
    list.clear();
    std::cout << "Hello World!\n";
}
