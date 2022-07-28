#include "Sample.h"
#include <iostream>
#include <queue>
#include <stack>
#include <list>
enum TTreeDir { T_LEFT=0, T_RIGHT, };
class TNode
{
public:
	int		m_iIndex;
	int		m_iDepth;
	TNode*	m_pChild[2];	
	TNode*  m_pParent;
	TNode() : m_iIndex(-1), m_iDepth(0)
	{
		for (int iNode=0; iNode < 2; iNode++)
		{
			m_pChild[iNode] = nullptr;
		}		
		m_pParent = nullptr;
	}
	virtual ~TNode()
	{		
		std::cout << this->m_iIndex << " ";
	}
};

TNode* g_pRootNode = nullptr;
int    g_iMaxDepth = 2;
int g_iCounter = 0;
TNode** g_ppNodeArray = nullptr;
TNode* g_pNodeArray[7] ;
std::queue<TNode*> g_Queue;

TNode* CreateNode(TNode* pParent)
{
	TNode* pNewNode = new TNode;
	pNewNode->m_iIndex = g_iCounter++;
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
	int iSelectLeft = 2 * pParent->m_iIndex + 1;
	int iSelectRight = 2 * ( pParent->m_iIndex + 1 );
	g_pNodeArray[iSelectLeft] = pParent->m_pChild[0];
	g_pNodeArray[iSelectRight] = pParent->m_pChild[1];
	Build(pParent->m_pChild[0]);
	Build(pParent->m_pChild[1]);
}
void Print(TNode* pParent)
{
	if (pParent == nullptr) return;
	//std::cout << " " << pParent->m_iIndex;
	Print(pParent->m_pChild[0]);	
	//std::cout << " " << pParent->m_iIndex;
	Print(pParent->m_pChild[1]);
	std::cout << " " << pParent->m_iIndex;
}
void DeleteAll(TNode* pParent)
{
	if (pParent == nullptr) return;	
	DeleteAll(pParent->m_pChild[0]);
	DeleteAll(pParent->m_pChild[1]);
	delete pParent;
	pParent = nullptr;	
}
void LevelOrderPrint(TNode* pParent)
{
	if (pParent == nullptr) return;
	std::cout << " " << pParent->m_iIndex;
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
void LevelOrderDeleteAll(TNode* pParent)
{
	if (pParent == nullptr) return;
	
	for (int iNode = 0; iNode < 2; iNode++)
	{
		if (pParent->m_pChild[iNode] != nullptr)
		{
			g_Queue.push(pParent->m_pChild[iNode]);
		}
	}
	
	delete pParent;
	pParent = nullptr;

	if (!g_Queue.empty())
	{
		TNode* pNode = g_Queue.front();
		g_Queue.pop();
		LevelOrderDeleteAll(pNode);
	}
}
void main()
{
	int iMaxNodeCounter = pow(2.0f, g_iMaxDepth + 1) - 1;
	g_ppNodeArray = new TNode*[iMaxNodeCounter];

	g_pRootNode = CreateNode(nullptr);	
	g_ppNodeArray[0] = g_pRootNode;
	g_pNodeArray[0] = g_pRootNode;

	Build(g_pRootNode);
	//LevelOrderPrint(g_pRootNode);
	Print(g_pRootNode);
	std::cout << std::endl;
	//LevelOrderDeleteAll(g_pRootNode);
	DeleteAll(g_pRootNode);

	delete[] g_ppNodeArray;
}