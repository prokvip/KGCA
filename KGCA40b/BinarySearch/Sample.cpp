#include <iostream>
#include <queue>
using namespace std;

struct Node
{
	int iDepth;
	int iValue;	
	Node* pLeft;//pChild[0]
	Node* pRight;//pChild[1]
	Node() {
		iValue = -1;
		pLeft = nullptr;
		pRight = nullptr;
	};
	Node(int i)
	{
		iValue = i;
		pLeft = nullptr;
		pRight = nullptr;
	}
	~Node()
	{
		delete pLeft;
		delete pRight;
	}
};
Node* g_pArray[7];
std::queue<Node*> g_Queue;
int g_iValue = 0;
void CreaeBinaryNode(Node* pNode)
{
	if (pNode->iDepth == 2) return;
	pNode->pLeft = new Node(++g_iValue);
	//g_pArray[g_iValue] = pNode->pLeft;
	pNode->pRight = new Node(++g_iValue);
	//g_pArray[g_iValue] = pNode->pRight;
	pNode->pLeft->iDepth = pNode->iDepth + 1;
	pNode->pRight->iDepth = pNode->iDepth + 1;
	CreaeBinaryNode(pNode->pLeft);
	CreaeBinaryNode(pNode->pRight);
}

void BinaryNodePrintPreOrder(Node* pNode)
{
	if (pNode == nullptr) return;
	std::cout << pNode->iValue;
	BinaryNodePrintPreOrder(pNode->pLeft);
	BinaryNodePrintPreOrder(pNode->pRight);
}
void BinaryNodePrintInOrder(Node* pNode)
{
	if (pNode == nullptr) return;	
	BinaryNodePrintInOrder(pNode->pLeft);
	std::cout << pNode->iValue;
	BinaryNodePrintInOrder(pNode->pRight);
}
void BinaryNodePrintPostOrder(Node* pNode)
{
	if (pNode == nullptr) return;	
	BinaryNodePrintPostOrder(pNode->pLeft);
	BinaryNodePrintPostOrder(pNode->pRight);
	std::cout << pNode->iValue;
}
// cnt = 2^detph+1 - 1, 7
//int iLeft = 2 * pNode->iValue + 1;
//int iRight = 2 * (pNode->iValue + 1);
//int iParent = (pNode->iValue - 1) / 2;
//          root(0)d=0
//    left(1)d=1       right(2)d=1
// L(3)d=2    R(4)d=2  L(5)d=2      R(6)d=2
// 
//          root(preorder)
//   left(1)       right(4)
// L(2)    R(3)  L(5)      R(6)

void BinaryNodePrintLevelOrder(Node* pNode)
{
	do {
		if (pNode->pLeft != nullptr)
			g_Queue.push(pNode->pLeft);
		if (pNode->pRight != nullptr)
			g_Queue.push(pNode->pRight);

		if (g_Queue.empty()) break;
		pNode = g_Queue.front();
		std::cout << pNode->iValue;
		g_Queue.pop();

		if (g_Queue.empty()) break;
		pNode = g_Queue.front();
	} while (pNode);
}
void main()
{	
	Node* pRoot = new Node(0);
	g_pArray[0] = pRoot;
	pRoot->iDepth = 0;
	CreaeBinaryNode(pRoot);
	BinaryNodePrintPreOrder(pRoot);
	std::cout << std::endl;
	BinaryNodePrintInOrder(pRoot);
	std::cout << std::endl;
	BinaryNodePrintPostOrder(pRoot);
	std::cout << std::endl;

	g_Queue.push(pRoot);
	BinaryNodePrintLevelOrder(pRoot);

	delete pRoot;
}