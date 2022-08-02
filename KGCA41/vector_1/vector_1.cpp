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
struct IsCmp
{
    int m_iValue;
    IsCmp(int value) : m_iValue(value) {}
    bool operator()(const TItem* item) 
    {
        return (item->iValue == m_iValue);
    }
};
struct IsCmp2
{
    int m_iValue;
    IsCmp2(int value) : m_iValue(value) {}
    bool operator()(const TItem& item)
    {
        return (item.iValue == m_iValue);
    }
    bool operator()(const TItem* item)
    {
        return (item->iValue == m_iValue);
    }
};
bool  compareInt(const int& a, const int& b)
{
    return a < b;
}
bool  compare(const TItem& a, const TItem& b)
{
    return a.iValue < b.iValue;
}
bool  comparePtr( TItem* a, TItem* b)
{
    return a->iValue < b->iValue;
}
int g_iFindValue;
bool  find_TItem(const TItem& a)
{
    return a.iValue == g_iFindValue;
}

int   Gen()
{
    static int iCount = 0;
    return iCount++;
}

int Sum10(int i){ return i + 10; };

int main()
{
    /// <summary>
    /// ///////////////////////////////
    /// </summary>
    /// <returns></returns>
    std::vector<int>  listIntA(20,7);    

    std::vector<int>  listInt;
    //listInt.reserve(20); // 메모리 확보,예약
    listInt.resize(20);
    for (int i = 0; i < listInt.size(); i++)
    {
        listInt[i] = i;
    }
    for (int i = 0; i < 20; i++)
    {
        listInt.push_back(i);
    }
    std::vector<int>  listIntB(listInt);
    std::vector<int>  listIntC(listInt.begin(), listInt.end());
    for (int i = 0; i < 5; i++)
    {
        listInt.insert(listInt.begin(),10+i);
    }
    for (int i = 0; i < 5; i++)
    {
        listInt.insert(listInt.end(), 20 + i);
    }
    int iValue1 = listInt.front();
    int iValue2 = listInt.back();
    listInt.pop_back();

    for (std::vector<int>::iterator iter = listInt.begin();
        iter != listInt.end();
        iter++)
    {
        //int* iValue = iter._Ptr; // *iter        
        std::cout << " " << *iter;
    }
    std::cout << std::endl;
    for (std::vector<int>::reverse_iterator iter = listInt.rbegin();
        iter != listInt.rend();
        iter++)
    {
        //int* iValue = iter._Ptr; // *iter        
        std::cout << " " << *iter;
    }
    std::cout << std::endl;
    for (auto iter = listInt.begin();
        iter != listInt.end();
        iter++)
    {
        //int* iValue = iter._Ptr; // *iter        
        std::cout << " " << *iter;
    }
    std::cout << std::endl;
    for (int i = 0; i < listInt.size(); i++)
    {
        std::cout << " " << listInt[i];
    }
    std::cout << std::endl;
    for (int value : listInt) // 복사
    {
        value = 100;
        std::cout << " " << value;
    }
    std::cout << std::endl;
    /*for (int& value : listInt)
    {
        value = 100;
        std::cout << " " << value;
    }*/
    std::sort(listInt.begin(), listInt.end(), compareInt);
    std::vector<int>::iterator iterint = std::find(listInt.begin(), listInt.end(), 17);    
    std::vector<int>::iterator retiterint;
    if (iterint != listInt.end())
    {
        retiterint = listInt.erase(iterint);
    }
    // std::copy
    std::vector<int> copyvector; 
    std::copy(listInt.begin(), listInt.end(), 
        std::back_inserter(copyvector));
    // std::generate 
    // 특정 범위의 원소를 함수의 반환값으로 체운다.
    std::generate(listInt.begin(), listInt.end(), Gen);        
    // std::transform
    std::vector<int> result1;
    std::transform(listInt.begin(), listInt.end(),
        std::back_inserter(result1), Sum10);
        //[](int i) -> int { return i + 10; });
        //std::plus<>{});

    std::vector<int> result2;
    std::transform(listInt.begin(), listInt.end(), result1.begin(),
    std::back_inserter(result2), std::multiplies<>{});
    // std::merge
    std::vector<int> resultMerge;
    std::merge(result1.begin(), result1.end(),
                result2.begin(), result2.end(), 
        std::back_inserter(resultMerge));
    listInt.clear();
    /// <summary>
    /// //
    /// </summary>
    /// <returns></returns>
    std::vector<TItem>  listitem;
    for (int i = 0; i < 20; i++)
    {
        TItem item(i);
        listitem.push_back(item);
    }
    for (int i = 0; i < 5; i++)
    {
        TItem item(10 + i);
        listitem.insert(listitem.begin(), item);
    }
    for (int i = 0; i < 5; i++)
    {
        TItem item(20 + i);
        listitem.insert(listitem.end(), item);
    }
    std::sort(listitem.begin(), listitem.end(), compare);
    //// find_if -> operator ()
    std::vector<TItem>::iterator iter2 = std::find_if(listitem.begin(), 
                                                    listitem.end(), IsCmp2(17));
    if (iter2 != listitem.end())
    {
        listitem.erase(iter2);
    }
    TItem finditem3(20);
    std::vector<TItem>::iterator iteritem =
        std::find(listitem.begin(), listitem.end(), finditem3);
    if (iteritem != listitem.end())
    {
        listitem.erase(iteritem);
    }
    listitem.clear();

    

    /// <summary>
    /// ////
    /// </summary>
    /// <returns></returns>
    std::vector<TItem*>  listptr;
    for (int i = 0; i < 20; i++)
    {
        listptr.push_back(new TItem(i));
    }
    for (int i = 0; i < 5; i++)
    {
        listptr.insert(listptr.begin(), new TItem(10 + i));
    }
    for (int i = 0; i < 5; i++)
    {
        listptr.insert(listptr.end(), new TItem(20 + i));
    }
    std::sort(listptr.begin(), listptr.end(), comparePtr);
    // find_if -> operator ()
    std::vector<TItem*>::iterator iterptr = std::find_if(listptr.begin(), listptr.end(), IsCmp2(17));
    if (iterptr != listptr.end())
    {
        delete *iterptr;
        listptr.erase(iterptr);
    }

    for (int i = 0; i < listptr.size(); i++)
    {
        delete listptr[i];
    }
    listptr.clear();
    std::cout << "Hello World!\n";
}
