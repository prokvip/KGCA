#include "TStudentManager.h"
// 함수템플릿, 
// 클래스템플릿
//void Fun1(int a = 9)
//{
//    std::cout << a;
//}
//void Fun1(int* a=nullptr)
//{
//    std::cout << *a;
//}
//void swap(int x, int y)
//{
//    int temp;
//    temp = x;
//    x = y;
//    y = temp;
//}
//void swap(int& x, int& y)
//{
//    int temp;
//    temp = x;
//    x = y;
//    y = temp;
//}
//void swap(int* a, int* b)
//{
//    int temp;
//    temp = *a;
//    *a = *b;
//    *b = temp;
//}
//void swap(float& x, float& y)
//{
//    int temp;
//    temp = x;
//    x = y;
//    y = temp;
//}
//void swap(float* a, float* b)
//{
//    int temp;
//    temp = *a;
//    *a = *b;
//    *b = temp;
//}
template<class T>
void Swap(T& a, T& b)
{
    T temp;
    temp = a;
    a = b;
    b = temp;
}
// 템플릿 예외
template<>
void Swap(char& a, char& b)
{
    char temp;
    temp = a;
    a = b;
    b = temp;
}
template<>
void Swap(short& a, short& b)
{
    short temp;
    temp = a;
    a = b;
    b = temp;
}

template<class T>
void Swap(T* a, T* b)
{
    T* temp;
    temp = a;
    a = b;
    b = temp;
}

template<>
void Swap(char* a, char* b)
{
    char* temp;
    temp = a;
    a = b;
    b = temp;
}
template<>
void Swap(short* a, short* b)
{
    short* temp;
    temp = a;
    a = b;
    b = temp;
}
//'void Swap(char *,char *)'
//이(가) 함수 템플릿의 특수화가 아닙니다.
void Swap( char a[],  char b[])
{
    int aLength1 = sizeof(a);
    int aLength2 = strlen(a);

    //char* temp;
    //temp = a;
    //a = b;
    //b = temp;
}
//void Swap(char[], char[])' 함수에 이미 본문이 있습니다.
void Swap(const char* a, const char* b)
{
    int aLength1 = sizeof(a);
    int aLength2 = strlen(a);

    //char* temp;
    //temp = a;
    //a = b;
    //b = temp;
}
TStudent Func()
{
    int a = 88;
    TStudent s(a,"aaa");
    return s;
}
//void Function(TStudent src)
//{
//    std::cout << src.m_iID;
//}
void Function(TStudent* src)
{
    std::cout << src->m_iID;
}
void Function(TStudent& src)
{
    std::cout << src.m_iID;
}
void Function(TStudent&& src)
{
    std::cout << src.m_iID;
}
int main()
{       
    TStudent t1;
    TStudent t2 = t1;
    TStudent t5 = TStudent(7);    
    TStudent&& t6 = TStudent(8);
    TStudent t7 = Func();
    TStudent t8 = std::move(TStudent(9));
    TStudent&& t9 = std::move(TStudent(11));


    TStudent t3,t4;
    t3 = t4;
    t3 = TStudent(1);

    TStudent* p = new TStudent;
    Function(*p);
    Function(std::move(*p));
    TStudent s;
    Function(s);
    int a = 4;
    int& b = a;   
    Function(TStudent());
    int&& i = 3;

    TStudentManager mgr;
    mgr.Init();
    mgr.Run();
    printf("아무키나 누르시오!");
    _getche();
}

