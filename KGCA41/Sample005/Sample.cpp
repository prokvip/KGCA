#include "TStudentManager.h"
#include "TItemManager.h"
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
int main()
{   
    ///*int a = 7;
    //int b = 5;
    //Swap(a, b);
    //float fa = 7;
    //float fb = 5;
    //Swap(fa, fb);
    //char ca = 7;
    //char cb = 5;
    //Swap(ca, cb);
    //short ca1 = 7;
    //short cb1 = 5;
    //Swap(ca1, cb1);*/
    //char strA[] = "aaa";
    //char strB[] = "bbb";
    //Swap(strA, strB);
    //char* pStrA = strA;
    //char* pStrB = strB;
    //Swap(pStrA, pStrB);
    ////int b;
    ////int* a=&b;
    ////*a = 877;
    ////Fun1(*a);

    ////int a = 7;
    ////int b = 5;
    ////swap(a, b);
    ////swap(&a, &b);

    ////float fa = 7.0f;
    ////float fb = 5.0f;
    ////swap(fa, fb);
    ////swap(&fa, &fb);

   /* TItemManager mgr;
    mgr.Init();
    mgr.Run();*/

    TStudentManager mgr;
    mgr.Init();
    mgr.Run();
    printf("아무키나 누르시오!");
    _getche();
}

