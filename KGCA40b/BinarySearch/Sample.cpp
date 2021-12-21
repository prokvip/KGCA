#include <iostream>
#include <queue>
using namespace std;

struct Node
{
	int iDepth;
	int iValue;	
	Node* pChild[2];
	Node() {
		iValue = -1;
		pChild[0] = nullptr;
		pChild[1] = nullptr;
	};
	Node(int i) : Node()
	{
		iValue = i;
		pChild[0] = nullptr;
		pChild[1] = nullptr;
	}
	~Node()
	{
		delete pChild[0];
		delete pChild[1];
	}
};
Node** g_ppArray=nullptr;
std::queue<Node*> g_Queue;
int g_iValue = 0;
void CreaeBinaryNode(Node* pNode)
{
	if (pNode->iDepth == 2) return;
	pNode->pChild[0] = new Node(++g_iValue);
	g_ppArray[g_iValue] = pNode->pChild[0];
	pNode->pChild[1] = new Node(++g_iValue);
	g_ppArray[g_iValue] = pNode->pChild[1];
	
	pNode->pChild[0]->iDepth = pNode->iDepth + 1;
	pNode->pChild[1]->iDepth = pNode->iDepth + 1;
	CreaeBinaryNode(pNode->pChild[0]);
	CreaeBinaryNode(pNode->pChild[1]);
}

void BinaryNodePrintPreOrder(Node* pNode)
{
	if (pNode == nullptr) return;
	std::cout << pNode->iValue;
	BinaryNodePrintPreOrder(pNode->pChild[0]);
	BinaryNodePrintPreOrder(pNode->pChild[1]);
}
void BinaryNodePrintInOrder(Node* pNode)
{
	if (pNode == nullptr) return;	
	BinaryNodePrintInOrder(pNode->pChild[0]);
	std::cout << pNode->iValue;
	BinaryNodePrintInOrder(pNode->pChild[1]);
}
void BinaryNodePrintPostOrder(Node* pNode)
{
	if (pNode == nullptr) return;	
	BinaryNodePrintPostOrder(pNode->pChild[0]);
	BinaryNodePrintPostOrder(pNode->pChild[1]);
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
	std::cout << pNode->iValue;	
	do {
		if (pNode->pChild[0] != nullptr)
			g_Queue.push(pNode->pChild[0]);
		if (pNode->pChild[1] != nullptr)
			g_Queue.push(pNode->pChild[1]);

		if (g_Queue.empty()) break;
		pNode = g_Queue.front();
		std::cout << pNode->iValue;
		g_Queue.pop();
	} while (pNode);
}
void main()
{	
	int iMaxDepth = 2;
	int cnt = pow(2, iMaxDepth + 1) - 1;
	g_ppArray = new Node*[cnt];

	Node* pRoot = new Node(0);	
	if (pRoot == nullptr) return;

	pRoot->iDepth = 0;
	g_ppArray[0] = pRoot;
	CreaeBinaryNode(pRoot);
	BinaryNodePrintPreOrder(pRoot); 	std::cout << std::endl;
	BinaryNodePrintInOrder(pRoot);		std::cout << std::endl;
	BinaryNodePrintPostOrder(pRoot);	std::cout << std::endl;
	BinaryNodePrintLevelOrder(pRoot);
	delete pRoot;
}