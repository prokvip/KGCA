#pragma once
#include "TNode.h"

class TQuadtree
{
public:
	TNode* m_pRootNode;
	int		m_iMaxDepth;
	TMap* m_pMap = nullptr;
	TCamera* m_pCamera = nullptr;
	std::vector<TNode*> m_pLeafNodeList;
	std::vector<TNode*> m_pDrawLeafNodeList;
public:
	bool	Create(TCamera* pMainCamera, TMap* m_pMap, int iMaxDepth = 3);
	bool	AddObject(TObject3D* pObj);
	void	BuildTree(TNode* pNode);
	bool	IsSubDivide(TNode* pNode);
	TNode* FindNode(TNode* pNode, TObject3D* pObj);
	void	Reset(TNode* pNode);
	bool	Frame();
	bool	Render();
	bool	Release();
	TNode*  VisibleNode(TNode* pNode);
	virtual ~TQuadtree();
};

