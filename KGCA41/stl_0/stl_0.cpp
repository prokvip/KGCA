#include <iostream>
#include <list>
#include <vector> // 배열(재할당되는 배열)
#include <array>  // 배열(크기가 고정)
#include <algorithm>
class TUser
{
public:
    int  m_id = 0;
    char m_szName[30] = { 0, };
    TUser(const TUser& d)
    {
        m_id = d.m_id;
        std::cout << "User(TUser& d)" << m_id << "\n";
    }
    TUser& operator = (TUser& data) 
    {
        m_id = data.m_id;
        std::cout << "operator =" << m_id << "\n";
    }
    TUser& operator = (TUser&& data)
    {
        m_id = data.m_id;
        std::cout << "operator = (TUser&& data" << m_id << "\n";
        return *this;
    }
    TUser(TUser&& data) 
    {
        m_id = data.m_id;
        std::cout << "TUser(TUser&& data)" << m_id << "\n";
    }
    TUser(int id, const char* name, int iSize) : m_id(id)
    {
        memset(m_szName, 0, sizeof(char) * 30);       
        memcpy(m_szName, name, sizeof(char) * iSize);
        std::cout << "TUser" << m_id << "\n";
    }
    TUser(int id) : m_id(id)
    {
        memset(m_szName, 0, sizeof(char) * 30);
        std::cout << "TUser" << m_id << "\n";
    }
    TUser() = default;
    ~TUser()
    {
        std::cout << "~TUser" << m_id << "\n";
    }
    static void Print(TUser& user)
    {
        std::cout << user.m_szName << "\n";
    }
};

TUser Add()
{
    TUser user;
    return user;
}
struct IsCmp
{
    int value;
    IsCmp(int i) : value(i) {}
    bool operator() (int& data)
    {
        return value == data;
    }
};
bool CompareInt(const int& a, const int &b)
{
    return a < b;
}

static auto cmp = [](const int& a){  return 41 == a;};

int main()
{    
    std::vector<int> listInt;
    listInt.reserve(100);
    for (int i = 0; i < 5; i++)
    {        
        listInt.push_back(rand()%100);
    }
    for (int i = 0; i < 5; i++)
    {
        listInt.insert(listInt.end(), rand() % 100);
    }
    int iValue1 = listInt.front();
    int iValue2 = listInt.back();
    listInt.pop_back();
   
    std::sort(listInt.begin(), listInt.end(), std::less<>());
    std::sort(listInt.begin(), listInt.end(), std::greater<>());
    std::sort(listInt.begin(), listInt.end(), CompareInt);

    int iRef = 9;
    std::vector<int>::iterator ifind1 =
        std::find(listInt.begin(), listInt.end(), 41);
    auto ifind2 =
        std::find_if(listInt.begin(), listInt.end(), IsCmp(41));
    
    
    auto ifind3 =
        std::find_if(listInt.begin(), listInt.end(), 
            cmp
        );


    auto ifind4 =
        std::find_if(listInt.begin(), listInt.end(),
            [&listInt](const int& a)
            {
                return 41 == a;
            }
        );
    

    if (ifind1 != listInt.end())
    {
        listInt.erase(ifind1);
    }
    if (ifind2 != listInt.end())
    {
        listInt.erase(ifind1);
    }
    //std::vector<TUser*>  list;    
    ////list.resize(26);
    //list.reserve(100);
    //for (int i = 0; i < 50; i++)
    //{
    //    list.emplace(list.end());
    //    list.emplace(list.end(), i);
    //    list.emplace_back(i, "kgca", _countof("kgca"));
    //    list.emplace_back(Add());
    //}
    //for (int i = 0; i < 26; i++)
    //{
    //    TUser* user = new TUser;
    //    user->m_szName[0] = 65 + i;
    //    list.push_back(user);
    //}
    //using Iter = std::vector<TUser*>::iterator;
    //for (Iter iter = list.begin();
    //    iter != list.end();
    //    iter++)
    //{
    //    TUser* user = (*iter);
    //    std::cout << user->m_szName << "\n";        
    //}
    //for (auto& user : list)
    //{
    //    TUser::Print(*user);
    //}    
    //

    //using rIter = std::vector<TUser>::reverse_iterator;
    //for (rIter iter = list.rbegin();
    //    iter != list.rend();
    //    iter++)
    //{
    //    std::cout << (*iter).m_szName << "\n";
    //}
    //for (Iter iter = list.begin();
    //    iter != list.end();
    //    iter++)
    //{
    //    std::cout << (*iter)->m_szName << "\n";
    //}
    //for (int i = 0; i < list.size(); i++)
    //{
    //    std::cout << list[i]->m_szName << "\n";
    //}
    //for (int i = list.size()-1; i >= 0; --i)
    //{
    //    std::cout << list[i]->m_szName << "\n";
    //}


    //for (Iter iter = list.begin();
    //    iter != list.end();
    //    )
    //{
    //    TUser* user = *iter;
    //    delete user;
    //    iter = list.erase(iter);
    //}

    //listInt.clear();
    std::cout << "Hello World!\n";
}
