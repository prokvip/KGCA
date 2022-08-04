#include <iostream>
#include <queue>
using namespace std;
struct cmp
{
    bool operator()(int a, int b)
    {
        return a > b;
    }
};


struct Info
{
    float x;
    int  y;
    Info()
    {

    }
    Info(float x, int y)
    {
        this->x = x;
        this->y = y;
    }
    bool operator < (const Info& a) const
    {
      return this->x <= a.x;
    }    
};
struct cmpInfoPointer {
    bool operator()(Info* a, Info* b)
    {
        return a->x <= b->x;
    }
};
struct cmpInfo {
    bool operator()(Info& a, Info& b)
    {
        return a.x <= b.x;
    }
};
int main()
{
    std::priority_queue<int> q;
    q.push(3);
    q.push(6);
    q.push(1);
    q.push(8);
    q.push(5);
    while (!q.empty())
    {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::priority_queue<int,
        std::vector<int>,
        cmp > q1;
                        //std::greater<int>> q1;
                        //std::less<int>> q1;
    q1.push(3);
    q1.push(6);
    q1.push(1);
    q1.push(8);
    q1.push(5);
    while (!q1.empty())
    {
        std::cout << q1.top() << " ";
        q1.pop();
    }
    std::cout << "Hello World!\n";

    priority_queue<Info, vector<Info>, cmpInfo> vec;
    Info data(0.1f,4);
    vec.push(data);
    vec.push({ 0.4f, 1 });
    vec.push({ 0.2f, 2 });
    vec.push({ 0.5f, 3 });
    while (!vec.empty())
    {
        cout << vec.top().x << vec.top().y << '\n';
        vec.pop();
    }
    //rror C2676 : 이항 '<' : 'const _Ty'이(가) 이 연산자를 정의하지 않거나 미리 정의된 연산자에
    //클래스 템플릿 멤버 함수 'bool std::less<_Ty>::operator ()(const _Ty &,const _Ty &) const
    priority_queue<Info> vec1;
    Info data1(0.1f, 4);
    vec1.push(data1);
    Info data2(0.3f, 4);
    vec1.push(data2);

    data1.x = 4.0f;
    data1.y = 9;

    vec1.push({ 0.4f, 1 });
    vec1.push({ 0.2f, 2 });
    vec1.push({ 0.5f, 3 });
    while (!vec1.empty())
    {
        cout << vec1.top().x << vec1.top().y << '\n';
        vec1.pop();
    }
}
