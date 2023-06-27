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
int main()
{   
   TStudentManager mgr;
    mgr.Init();
    mgr.Run();
    printf("아무키나 누르시오!");
    _getche();
}

