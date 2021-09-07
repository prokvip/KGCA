// BST.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
struct TNode
{
	int	   m_iDepth;
	int    m_iValue;
	TNode* m_pChildNode[2];
	TNode();
	TNode(int iValue);
	~TNode();
};
TNode::TNode() : m_iDepth(-1), m_iValue(-1)
{	
	m_pChildNode[0] = nullptr;
	m_pChildNode[1] = nullptr;
}
TNode::TNode(int iValue) : m_iDepth(-1), m_iValue(iValue)
{
	m_pChildNode[0] = nullptr;
	m_pChildNode[1] = nullptr;
}
TNode::~TNode()
{
	int iDelData = this->m_iValue;
	if (m_pChildNode[0] != nullptr)
	{
		delete m_pChildNode[0];
	}
	if (m_pChildNode[1] != nullptr)
	{
		delete m_pChildNode[1];
	}
	std::cout << "Current :" << iDelData << "Delete!\n";
}
int   g_iData[10] = { 5,0,4,7,8,3,9,2,6,1 };
TNode* g_pRoot = nullptr;

void AddBinaryNode(TNode* pNode, int iValue)
{
	if (pNode->m_iValue > iValue)
	{
		if (pNode->m_pChildNode[0] == nullptr)
		{
			pNode->m_pChildNode[0] = new TNode(iValue);
			//pNode->m_pChildNode[0]->m_iValue = iValue;
			pNode->m_pChildNode[0]->m_iDepth = pNode->m_iDepth + 1;
		}
		else
		{
			AddBinaryNode(pNode->m_pChildNode[0], iValue);
		}
	}
	else
	{
		if (pNode->m_pChildNode[1] == nullptr)
		{
			pNode->m_pChildNode[1] = new TNode(iValue);
			//pNode->m_pChildNode[1]->m_iValue = iValue;
			pNode->m_pChildNode[1]->m_iDepth = pNode->m_iDepth + 1;
		}
		else
		{
			AddBinaryNode(pNode->m_pChildNode[1], iValue);
		}
	}	
}
TNode* Find(TNode* pNode, int iFind);

int main()
{
	g_pRoot = new TNode(g_iData[0]);
	//g_pRoot->m_iValue = g_iData[0];
	g_pRoot->m_iDepth = 0;
	for (int i = 1; i < 10; i++)
	{
		AddBinaryNode(g_pRoot, g_iData[i]);
	}

	TNode* pFindNode = Find(g_pRoot, 8);
	if (pFindNode != nullptr)
	{
		std::cout << pFindNode->m_iValue << "Hello World!\n";
	}
	else
	{
		std::cout << "Not! Finder!\n";
	}
	delete g_pRoot;
}

TNode* Find(TNode* pNode, int iValue)
{
	if (pNode->m_iValue == iValue)
	{
		return pNode;
	}
	if (pNode->m_iValue > iValue)
	{
		if (pNode->m_pChildNode[0] != nullptr)
		{	
			return Find(pNode->m_pChildNode[0], iValue);
		}		
	}
	else
	{
		if (pNode->m_pChildNode[1] != nullptr)
		{
			return Find(pNode->m_pChildNode[1], iValue);
		}
	}
	return nullptr;
}