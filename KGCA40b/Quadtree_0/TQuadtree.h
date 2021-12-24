#pragma once
#include "TNode.h"

class TQuadtree
{
public:
	int		m_iWidth;
	int		m_iHeight;
	int		m_iMaxDepth;
	TNode*  m_pRootNode;
	
public:
	std::vector<TNode*> g_ppArray;
	//TNode** g_ppArray = nullptr;
	std::queue<TNode*> g_Queue;	
	int g_iValue = 0;
public:
	void		Init(int iWidth, int iHeight, int iMaxDepth);
	TNode*		CreateNode(TNode* pParent, float x, float y, float w, float h);
	void		BuildTree(TNode* pParent);
	bool		AddObject(TObject* obj);
	TNode*		FindNode(TNode* pNode, int x, int y);
	TNode*		FindNode(TNode* pNode, TRect rt);
public:
	void PrintObjectList(TNode* pNode);
//	void BinaryNodePrintInOrder(TNode* pNode);
//	void BinaryNodePrintPostOrder(TNode* pNode);
//	void BinaryNodePrintLevelOrder(TNode* pNode);
};
