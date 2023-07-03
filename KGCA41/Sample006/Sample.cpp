#include "TStd.h"

struct TWidget
{
    int m_iID;
    int a;
    int b;
    int c;
    int d;
    int e;
    TWidget()
    {
        m_iID = 0;
        a = b = c = d = e = 999;
    }
    ~TWidget()
    {
        std::cout << "~TWidget";
    }
    //friend std::ostream& operator << (  std::ostream& os, 
    //                                    const TWidget& s);
};
template<typename t1, typename t2>
auto add(t1 a, t2 b)-> decltype(a+b)
{
    return (int)(a + b);
}
// 새로운 함수 정의 문법
auto Get() -> int
{
    float i = 9.3f;
    return i+4;
}
int GetInt()
{
    int i = 9;
    return i;
}
auto PRINT() -> void
{
    std::cout << "kgca";
}

int main()
{  
    int a = 10;
    int b = 20;
    int c = a + b;
    auto fun = [](int i, int j) 
    { 
        return i + j; 
    };
    int iRet = fun(a,b);
    //auto GetInt() -> int
    //무명함수
    PRINT(); // 캡쳐절
    auto Fun1 = []()->void{ std::cout << "kgca"; };
    auto Fun2 = []() { std::cout << "kgca"; };
    auto Fun3 = [] { std::cout << "kgca"; };

    Fun3();
    Fun3();

    Fun3();
    Fun3();
    Fun3();
    Fun3();
    Fun3();
    Fun1();

    int jValue = Get();
    std::cout << jValue;

    int i = 9;
    // 새로운 변수가 생성된다.
    auto Function1 = [i]() { return i; };
    int iValue1 = Function1();
    std::cout << iValue1;
    // 참조로 처리된다.
    auto Function2 = [&i]() -> int { return i += 1; };
    int iValue2 = Function2();
    std::cout << iValue2;
    

    //// 컴파일러 결정(추론)
    //auto ret = Get();
    ////int (*Func)() = Get;
    //auto Func = Get;
    //Func();
    //// // 컴파일러 결정(추론)
    //decltype(3) myInt = 5;
    //decltype("str") myString = "str";
    //decltype(3.14) myFloat = 3.14;


    TLinkedlist<TWidget> list;
    list.Init();
    for (int i = 0; i < 3; i++)
    {
        TNode<TWidget>* pNewNode = new TNode<TWidget>;
        pNewNode->m_pData = new TWidget;
        pNewNode->m_pData->a = i;
        list.Push_Back(pNewNode);
    }   
    list.Release();

    printf("아무키나 누르시오!");
    _getche();
}

