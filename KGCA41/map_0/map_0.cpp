#include <iostream>
#include <map> // 배열
#include <unordered_map> // 배열
#include <vector> // 배열
#include <algorithm> // 배열
#include <iterator>
class TItem
{
public:
    int iValue;
    int jValue;
    //bool operator == (const TItem& p)
    //{
    //    return iValue == p.iValue;
    //}
    //TItem(const TItem& a)
    //{
    //    iValue = a.iValue;
    //    std::cout << "TItem(const TItem& a)\n ";
    //}
    //TItem(const TItem&& a)
    //{
    //    iValue = a.iValue;
    //    std::cout << "const TItem&& a\n ";
    //}
    TItem(int i) : iValue(i)
    {
        std::cout << "TItem(int i)\n ";
    }
    TItem(int i, int j) : iValue(i), jValue(j)
    {
        std::cout << "TItem(int i)\n ";
    }
    TItem() {}
    ~TItem()
    {
        std::cout << " ~TItem()\n ";
    }
};
int main()
{
    //std::pair<int, int> arr;
    //std::vector< std::pair<int, int> > a;
    //std::tuple<int, int, int> ar32;

    //std::map<std::string, int> datalist;
    //datalist["홍길동"] = 10;
    //datalist["김자홍"] = 20;
    //datalist["홍길동"] = 30;
    //std::map<std::string, int>::iterator iter = 
    //    datalist.find("김자홍");
    //if (iter == datalist.end())
    //{
    //    std::cout << "Not! Find!\n";
    //}
    //else
    //{
    //    std::cout << "Ok! Find!\n";
    //}

    //// pair<key,value>    
    //std::map<int, TItem> list1;  
    //TItem item(11);
    //TItem item1(22);
    //TItem item2(33);
    //std::pair<std::map<int, TItem>::iterator, bool> ret;
    //ret = list1.insert(std::make_pair(0, item));
    //ret = list1.insert(std::make_pair(1, item));
    //ret = list1.insert(std::make_pair(0, item));
    //if (ret.second == false)
    //{
    //    std::cout << "Not! push!\n";
    //}
    //for (int i = 0; i < 3; i++)
    //{
    //    TItem item(22);
    //    /*std::pair<int, TItem> newPair(i, item);
    //    list1.insert(newPair);*/
    //    list1.insert(std::make_pair(0, item));
    //}
    //for (int i = 0; i < 1; i++)
    //{
    //    //error C2661: 'std::pair<const int,TItem>::pair'
    //    //: 오버로드된 함수에서 2개의 인수를 사용하지 않습니다.
    //     TItem item(i);
    //     //list1.emplace(3, item); 
    //     list1.emplace(std::make_pair(3, item));
    //}

    ///// <summary>
    ///// ///////
    ///// </summary>
    ///// <returns></returns>
    //std::map<int, TItem*> list2;    
    //for (int i = 0; i < 5; i++)
    // {         
    //     list2.insert(std::make_pair(i,new TItem(i) ));
    // }
    // for (int i = 0; i < list2.size(); i++)
    // {
    //     TItem* pData = list2[i];
    //     std::cout << pData->iValue << " ";
    // }
    // std::cout << "Hello World!\n";
    // for (std::map<int, TItem*>::iterator iter =
    //     list2.begin();
    //     iter != list2.end();
    //     iter++)
    // {
    //     int iKey       = iter->first;
    //     TItem* pData   = iter->second;
    //     std::cout << pData->iValue << " ";
    // }
    // for (std::pair<int, TItem*> data : list2)
    // {
    //     int iKey = data.first;
    //     TItem* pData = data.second;
    //     std::cout << pData->iValue << " ";
    // }
    // for (auto data : list2)
    // {
    //     int iKey = data.first;
    //     TItem* pData = data.second;
    //     std::cout << pData->iValue << " ";
    // }
    // std::cout << "Hello World!\n";


     //////////////////////////////////
     std::unordered_map<std::string, int> datalist;
     datalist["aaaa"] = 10;
     datalist["aaab"] = 10;
     datalist["aaab0"] = 10;
     datalist["aaab1"] = 10;
     datalist["홍길동5"] = 10;
     datalist["홍길동1"] = 10;
     datalist["김자홍"] = 20;
     datalist["홍길동"] = 30;
     std::unordered_map<std::string, int>::iterator iter =
         datalist.find("김자홍");
     if (iter == datalist.end())
     {
         std::cout << "Not! Find!\n";
     }
     else
     {
         std::cout << "Ok! Find!\n";
     }

     // pair<key,value>    
     std::unordered_map<int, TItem> list1;
     TItem item(11);
     TItem item1(22);
     TItem item2(33);
     std::pair<std::unordered_map<int, TItem>::iterator, bool> ret;
     ret = list1.insert(std::make_pair(0, item));
     ret = list1.insert(std::make_pair(1, item));
     ret = list1.insert(std::make_pair(0, item));
     if (ret.second == false)
     {
         std::cout << "Not! push!\n";
     }
     for (int i = 0; i < 3; i++)
     {
         TItem item(22);
         /*std::pair<int, TItem> newPair(i, item);
         list1.insert(newPair);*/
         list1.insert(std::make_pair(0, item));
     }
     for (int i = 0; i < 1; i++)
     {
         //error C2661: 'std::pair<const int,TItem>::pair'
         //: 오버로드된 함수에서 2개의 인수를 사용하지 않습니다.
         TItem item(i);
         //list1.emplace(3, item); 
         list1.emplace(std::make_pair(3, item));
     }

     /// <summary>
     /// ///////
     /// </summary>
     /// <returns></returns>
     std::map<int, TItem*> list2;
     for (int i = 0; i < 5; i++)
     {
         list2.insert(std::make_pair(i, new TItem(i)));
     }
     for (int i = 0; i < list2.size(); i++)
     {
         TItem* pData = list2[i];
         std::cout << pData->iValue << " ";
     }
     std::cout << "Hello World!\n";
     for (std::map<int, TItem*>::iterator iter =
         list2.begin();
         iter != list2.end();
         iter++)
     {
         int iKey = iter->first;
         TItem* pData = iter->second;
         std::cout << pData->iValue << " ";
     }
     for (std::pair<int, TItem*> data : list2)
     {
         int iKey = data.first;
         TItem* pData = data.second;
         std::cout << pData->iValue << " ";
     }
     for (auto data : list2)
     {
         int iKey = data.first;
         TItem* pData = data.second;
         std::cout << pData->iValue << " ";
     }
}
