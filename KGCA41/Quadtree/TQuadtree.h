#pragma once
#include "TNode.h"
class TQuadtree
{
public:
	TNode* m_pRootNode;
	int		m_iMaxDepth;
public:
	bool	Create(TRect rt, int iMaxDepth=2);
	bool	AddObject(TObject* pObj);
	void	BuildTree(TNode* pNode);
	bool	IsSubDivide(TNode* pNode);
	TNode*  FindNode(TNode* pNode, TObject* pObj);
	void	Reset(TNode* pNode);
};

