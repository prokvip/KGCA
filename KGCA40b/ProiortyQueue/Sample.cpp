#include <iostream>
#include <queue>
#include <vector>
#include <set>
using namespace std;
struct TNode
{
	int iValue;
};
struct CmpNode {
	bool operator()(TNode& a, TNode& b)
	{
		return a.iValue > b.iValue;
	}
};
// ���� ���
// std::priority_queue - > ������(�켱����), ��Ŷ ����(�켱����), �۾�(�켱����)
void main()
{
	// ���� ���- ������ �⺻�̴�.
	// ������ 100�� , ��� ��������? 
	// 10�۾� ��-> ����(3��), �ƺ�(6), ��(1)
	std::set<int> s1;
	for (int n : {1, 1, 1, 1, 1, 2, 9, 56})
	{
		s1.insert(n);
	}
	std::multiset<int> s2;
	for (int n : {1, 1, 1, 1, 1, 2, 9, 56})
	{
		s2.insert(n);
	}

	std::priority_queue<int> q;
	for (int n : {1, 9, 6, 7, 5, 2, 4, 56})
	{
		q.push(n);
	}
	while (!q.empty())
	{
		std::cout << q.top() << " ";
		q.pop();
	}

	std::priority_queue<TNode, std::vector<TNode>, CmpNode >  q1;
	q1.push({ 4 });
	q1.push({ 1 });
	q1.push({ 5 });	
	while (!q1.empty())
	{
		std::cout << q1.top().iValue << " ";
		q1.pop();
	}

	//����
	auto CMP = [](TNode& a, TNode& b)->bool 
	{ 
		return a.iValue <= b.iValue; 
	};

	std::priority_queue<TNode, std::vector<TNode>, decltype(CMP) >  q2(CMP);
	q2.push({ 4 });
	q2.push({ 1 });
	q2.push({ 5 });
	while (!q2.empty())
	{
		std::cout << q2.top().iValue << " ";
		q2.pop();
	}
}