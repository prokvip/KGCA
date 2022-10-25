#pragma once
#include "TNode.h"
#include "TMap.h"
class TQuadtree
{
public:
	TNode* m_pRootNode;
	int		m_iMaxDepth;
	TMap* m_pMap = nullptr;
public:
	bool	Create(TMap* m_pMap, int iMaxDepth = 2);
	bool	AddObject(TObject3D* pObj);
	void	BuildTree(TNode* pNode);
	bool	IsSubDivide(TNode* pNode);
	TNode* FindNode(TNode* pNode, TObject3D* pObj);
	void	Reset(TNode* pNode);
};

