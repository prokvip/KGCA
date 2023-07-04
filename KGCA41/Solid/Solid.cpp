#include <iostream>

// 클레스 설계의 기본 원칙(SOLID) -> c++ 이미 원칙을 사용하고 있다.
// 소프트웨어 공학(객체, Class)
//1) 단일 책임(Single Responsibility) 원칙
//   1개의 클래스는 고유한 클래스의 기능만을 구현해라.
//   1개의 클래스에 모두 구현 -> 링크드리스트, 학생, 학생메니저, 파일관리(저장,로드)
//   파일관리( TLinkedlist::Save() ), 정렬, 검색
//2) 개방-페쇄( Open-Cloed princple)원칙 -> 상속 + 가상함수
//   확장은 오픈이다,   변경은 닫혀있다.
//  -> 상속을 통해서 기존 코드는 그대로 두고 파생한 클래스에서 
//     기존코드에 해당하는 기능을 구현해서 재정이 할 수 있다. 첨부)결과적으로 조건절을 뺄수 있다.
//3) 리스코프 치환(Liskov substitution) 원칙
//   -기반클래스는 파생 클래스로 대체 할 수 있다.
//   -파생클래스는 언제나 자신의 부모 클래스를 교체 될 수 있다.
//    결과적으로 부모 클래스가 들어갈 자리에 자식 클래스를 넣어도 문제가 없이 작동한다.
// 4) 인터페이스 분리(Interface Segregatio) 원칙( 인터페이스, 추상클래스)
//   - 해당 클래스와 관련이 없는 기능까지 상속 받으면 안좋다. 관련된 기능만 상속될 수 있도록
//     인터페이스를 분리해서 해당 기능만 있는 인터페이스를 상속해서 써라.  
//     다수의 기능을 관련된 공통 기능을 분리해서 클래스로 만들어라. 
//     분리된 클래스에 해당하는 상속을 통해서 불필요한 데이터 및 기능을 안써도 된다.
// 5) 의존관계 역전 원칙(Dependency inversion principle)
//    부모 클래스를 대표해서 리스트로 관리하면 된다.


class THuman
{
    int     iType;
    char    name[100];
public :
    virtual void SetName() = 0;
};
class TPrint
{
    int     PrintCounter = 0;
public:
    virtual void Print() = 0;
};
class TPerson : public THuman
{
public:
    virtual void    SetName(const char _name[]) 
    {
        std::cout << "TPersonSetName";
    };
    int     subject;
    virtual void    Print()
    {
        std::cout << "TPersonPrint";
    };
};
class TPersonCharacter :  public THuman
{
  //  int     iType; // 0:TStudent, 1: TCharacter, 2: NPC
public:
    virtual void    SetName(const char _name[])
    {
        std::cout << "TPersonSetName";
    };
    int     Gamelevel;
    virtual void    SetLevel(int level)
    {
        std::cout << "TPersonSetLevel";
    }
    virtual void    Print()
    {
        std::cout << "TPersonPrint";
    };
};
class TPersonStudent : public THuman
{
public:
    virtual void    SetName()
    {
        std::cout << "TPersonSetName";
    };
    virtual void    Print()
    {
        std::cout << "TPersonPrint";
    };
};
class TStudent : public TPersonStudent
{
    int     id;
    int     subject;
public:    
    void    Set(int _id) { id = _id; }
    void    SetName() {};
    void    Print()
    {
        std::cout << id << subject << std::endl;
    }
    /*void    Save()
    {
        std::cout << s.id << std::endl;
    }*/
    TStudent(int id) { this->id = id; }
    TStudent() { }
};
class TCharacter : public TPersonCharacter
{
    int     id;
    int     Gamelevel;
public:
    void    Set(int _id) { id = _id; }
    void    SetLevel(int level) { Gamelevel = level; }
    void    SetName() {};
    void    Print()
    {
        std::cout << id << Gamelevel << std::endl;
    }
    /*void    Save()
    {
        std::cout << s.id << std::endl;
    }*/
    TCharacter(int id) { this->id = id; }
};
class TNpc :public TCharacter 
{
};
class Tpc :public TCharacter {public:};
struct TSaveStudent
{
    static void Save(TStudent& s) { std::cout << s.id << std::endl; }
};

class TLinkedList
{
public :    
    THuman*   m_pHead;
    virtual void   push_pront() {};
    virtual void   pop_pront() {}
    virtual void   push_back() {};
    virtual void   pop_back() {}
};
class TStack : private TLinkedList
{
public:
    virtual void   push_pront() {};
    virtual void   pop_pront() {}
};
int main()
{
    //TLinkedList list;
    ////main()->list->TStudent;
    ////main()->list-><THuman><-TStudent;


    //// N
    //TPerson* list[2];
    //list[1] = new TStudent(1);
    //list[2] = new TCharacter(1);

    ////N1
    //TStudent* s1 = (TStudent*)list[1];
    //// N2
    //TCharacter* s2 = (TCharacter*)list[2];

    //TPerson* p0 = s1;
    //TPerson* p1 = s2;

    //N = N1;
    //N = N2;
    //N1 = N;

    //for (int i = 0; i < 2; i++)
    //{
    //    list[i]->Print();
    //}

    //TStudent s1(3);
    //TSaveStudent::Save(s1);
    std::cout << "Hello World!\n";
}
