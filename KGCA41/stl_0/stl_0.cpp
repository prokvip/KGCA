#include <iostream>
#include <list>
#include <vector> // 배열(재할당되는 배열)
#include <array>  // 배열(크기가 고정)
#include <algorithm>
class TUser
{
public:
    int  m_id = 0;
    std::wstring  m_szName;
    TUser(const TUser& d)
    {
        m_id = d.m_id;
        std::cout << "User(TUser& d)" << m_id << "\n";
    }
    TUser& operator = (TUser& data) 
    {
        m_id = data.m_id;
        std::wcout << L"operator =" << m_id << std::endl;
    }
    TUser& operator = (TUser&& data)
    {
        m_id = data.m_id;
        m_szName = data.m_szName;
        std::wcout << L"operator = (TUser&& data" << m_id << std::endl;
        return *this;
    }
    TUser(TUser&& data) 
    {
        m_id = data.m_id;
        m_szName = data.m_szName;
        std::wcout << L"TUser(TUser&& data)" << m_id << std::endl;
    }
    TUser(int id, const wchar_t* name, int iSize) : m_id(id)
    {
        m_szName = name;
        std::wcout << L"TUser" << m_id << std::endl;
    }
    TUser(int id) : m_id(id)
    {
        std::wcout << L"TUser" << m_id << std::endl;
    }
    TUser() = default;
    ~TUser()
    {
        std::wcout << L"~TUser" << m_id << "\n";
    }
    static void Print(TUser& user)
    {
        std::wcout << user.m_szName << "\n";
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
    setlocale(LC_ALL, "korean");
    std::wstring b0 = L"a게임";
    std::wcout << b0;

    using StringArray = std::vector<std::wstring>;
    std::vector<StringArray> listString;
    std::vector<std::wstring> list0, list1;
    list0.push_back(L"kgca");
    list0.push_back(L"kgcar게임아카데미");
    list0.push_back(L"kgcar게임아카데미게임아카데미게임아카데미게임아카데미");
    list0.push_back(L"kgca");
    list0.push_back(L"kgcar게임아카데미");
    list0.push_back(L"kgcar게임아카데미게임아카데미게임아카데미게임아카데미");
    
    list1.emplace_back(L"kgca");
    list1.emplace_back(L"kgcar게임아카데미");
    list1.emplace_back(L"kgcar게임아카데미게임아카데미게임아카데미게임아카데미");
    list1.emplace_back(L"kgca");
    list1.emplace_back(L"kgcar게임아카데미");
    list1.emplace_back(L"kgcar게임아카데미게임아카데미게임아카데미게임아카데미");

   // listString.emplace_back(list0);
    //listString.emplace_back(list1);

    std::wstring s0 = list0[1];
    //std::wstring s1 = listString[1][2];
    std::wcout << s0 << std::endl;
    std::wcout << list0[1] << std::endl;

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
