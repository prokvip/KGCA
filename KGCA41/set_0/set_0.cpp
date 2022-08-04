// set_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <set>
#include <queue>
#include <stack>
int main()
{
    std::stack<int> s;
    s.push(4);
    s.push(3);
    s.push(65);
    s.push(1);
    std::cout << s.top(); 
    s.pop();
    int iLength = s.size();
    if (s.empty())
    {
        std::cout << "Hello World!\n";
    }

    std::queue<int> q;
    q.push(4);
    q.push(3);
    q.push(65);
    q.push(1);    
    while (!q.empty())
    {
        std::cout << q.front() << " ";
        q.pop();
    }

    std::set<int>  list;
    list.insert(8);
    list.insert(0);
    list.insert(1);
    list.insert(2);
    list.insert(1);

    std::multiset<int>  listMulit;
    listMulit.insert(8);
    listMulit.insert(0);
    listMulit.insert(1);
    listMulit.insert(2);
    listMulit.insert(1);
    int iNum = listMulit.count(1);



    std::cout << "Hello World!\n";
}
