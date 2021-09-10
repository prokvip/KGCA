#pragma once
#include "TNode.h"
#include <queue>
class TQuadtree
{
public:
	TNode*		m_pRootNode;
	int			m_iWidth;
	int			m_iHeight;
	std::queue<TNode*>  m_Queue;
public:
	bool    Init(int iWidth, int iHeight);
	TNode*  CreateNode(TNode* pParent, float x, float y, float w, float h);
	void	Buildtree(TNode*);
	bool    AddObject(TVector2 pos);
	TNode*  FindNode(TNode* pNode, TVector2 pos);
	void    Release();
public:
	TQuadtree();
	virtual ~TQuadtree();
};

