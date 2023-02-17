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
	TNode* FindNode(TNode* pNode, T_BOX tBox);
	void	Reset(TNode* pNode);
	UINT    SelectVertexList(T_BOX& tBox, std::vector<TNode*>& selectNodeList);
	bool	Frame();
	bool	Render();
	bool	Release();

	void    RenderObject(TNode* pNode);
	TNode*  VisibleNode(TNode* pNode);
	virtual ~TQuadtree();
};

