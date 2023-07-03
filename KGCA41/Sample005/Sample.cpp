#include "TStudentManager.h"
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

