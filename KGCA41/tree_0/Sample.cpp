#include "Sample.h"
#include <iostream>
#include <queue>
#include <stack>
#include <list>
enum TTreeDir { T_LEFT=0, T_RIGHT, };
class TNode
{
public:
	int		m_iData;
	int		m_iDepth;
	TNode*	m_pChild[2];	
	TNode*  m_pParent;
	TNode() : m_iData(-1), m_iDepth(0)
	{
		for (int iNode; iNode < 2; iNode++)
		{
			m_pChild[iNode] = nullptr;
		}		
		m_pParent = nullptr;
	}
	virtual ~TNode()
	{
		for (int iNode; iNode < 2; iNode++)
		{
			delete m_pChild[iNode];
		}		
		m_pChild[0] = nullptr;
		m_pChild[1] = nullptr;
		std::cout << this->m_iData << " ";
	}
};

TNode* g_pRootNode = nullptr;
int    g_iMaxDepth = 2;
int g_iCounter = 0;

std::queue<TNode*> g_Queue;

TNode* CreateNode(TNode* pParent)
{
	TNode* pNewNode = new TNode;
	pNewNode->m_iData = g_iCounter++;
	if (pParent != nullptr)
	{
		pNewNode->m_iDepth = pParent->m_iDepth + 1;
		pNewNode->m_pParent = pParent;
	}
	return pNewNode;
}
void Build(TNode* pParent)
{
	if (pParent->m_iDepth >= g_iMaxDepth) return;
	pParent->m_pChild[0] = CreateNode(pParent);
	pParent->m_pChild[1] = CreateNode(pParent);	
	Build(pParent->m_pChild[0]);
	Build(pParent->m_pChild[1]);
}
void Print(TNode* pParent)
{
	if (pParent == nullptr) return;
	//std::cout << " " << pParent->m_iData;
	Print(pParent->m_pChild[0]);	
	//std::cout << " " << pParent->m_iData;
	Print(pParent->m_pChild[1]);
	std::cout << " " << pParent->m_iData;
}
//void Delete(TNode* pParent)
//{
//	if (pParent == nullptr) return;	
//	Delete(pParent->m_pChild[0]);	
//	Delete(pParent->m_pChild[1]);
//	delete pParent;
//	pParent = nullptr;	
//}
void LevelOrderPrint(TNode* pParent)
{
	if (pParent == nullptr) return;
	std::cout << " " << pParent->m_iData;
	for (int iNode = 0; iNode < 2; iNode++)
	{
		if (pParent->m_pChild[iNode] != nullptr)
		{
			g_Queue.push(pParent->m_pChild[iNode]);
		}
	}
	if (!g_Queue.empty())
	{
		TNode* pNode = g_Queue.front();
		g_Queue.pop();
		LevelOrderPrint(pNode);
	}
}
//void LevelOrderDelete(TNode* pParent)
//{
//	if (pParent == nullptr) return;
//	
//	for (int iNode = 0; iNode < 2; iNode++)
//	{
//		if (pParent->m_pChild[iNode] != nullptr)
//		{
//			g_Queue.push(pParent->m_pChild[iNode]);
//		}
//	}
//	
//	delete pParent;
//	pParent = nullptr;
//
//	if (!g_Queue.empty())
//	{
//		TNode* pNode = g_Queue.front();
//		g_Queue.pop();
//		LevelOrderDelete(pNode);
//	}
//}
void main()
{
	g_pRootNode = CreateNode(nullptr);	
	Build(g_pRootNode);
	//LevelOrderPrint(g_pRootNode);
	Print(g_pRootNode);
	std::cout << std::endl;
	//LevelOrderDelete(g_pRootNode);
	//Delete(g_pRootNode);
	delete g_pRootNode;
}