#include <iostream>
#include <vector> // 배열
#include <algorithm> // 배열

class TItem
{
public:
    int iValue;
    int jValue;
    bool operator == (const TItem& p)
    {
        return iValue == p.iValue;
    }
    TItem(const TItem& a)
    {
        iValue = a.iValue;
        std::cout << "TItem(const TItem& a)\n ";
    }
    TItem(const TItem&& a)
    {
        iValue = a.iValue;
        std::cout << "const TItem&& a\n ";
    }
    TItem(int i) : iValue(i)
    {
        std::cout << "TItem(int i)\n ";
    }
    TItem(int i, int j) : iValue(i), jValue(j)
    {
        std::cout << "TItem(int i)\n ";
    }
    ~TItem()
    {
        std::cout << " ~TItem()\n ";
    }
};
int main()
{    
    std::vector<TItem> list1;   
    list1.reserve(10);
  /*  for (int i = 0; i < 2; i++)
    {     
        TItem item(22);
        list1.push_back(item);
    }*/
    /*for (int i = 0; i < 1; i++)
    {
        TItem item(i);
        list1.emplace(list1.end(), item);
    }*/
    /*std::cout << "Hello World!\n";
    for (int i = 0; i < 1; i++)
    {
        TItem item(i);
        list1.emplace_back(item);
    }*/
    std::cout << "Hello World!\n";
    for (int i = 0; i < 10; i++)
    {
        list1.emplace_back(i+10);
    }
    /// <summary>
    /// ///////
    /// </summary>
    /// <returns></returns>
    std::vector<TItem*> list2;   
    list2.reserve(10);
   /* for (int i = 0; i < 10; i++)
    {
        list2.push_back(new TItem(i, i+10));
    }*/
      /*for (int i = 0; i < 1; i++)
      {
          TItem item(i);
          list2.emplace(list2.end(), item);
      }
      std::cout << "Hello World!\n";
      for (int i = 0; i < 1; i++)
      {
          TItem item(i);
          list2.emplace_back(item);
      }*/
   std::cout << "Hello World!\n";
    for (int i = 0; i < 10; i++)
    {
        list2.emplace_back(new TItem(i,i+20));
    }
    std::cout << "Hello World!\n";
}
