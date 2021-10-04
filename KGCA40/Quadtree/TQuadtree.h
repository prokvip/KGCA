#pragma once
#include "TNode.h"
#include <queue>
#include "TCollision.h"
#include "TObject.h"
class TQuadtree
{
public:
	TNode*		m_pRootNode;
	int			m_iWidth;
	int			m_iHeight;
	std::queue<TNode*>  m_Queue;
	TObject player;
public:
	bool    Init(int iWidth, int iHeight);
	TNode*  CreateNode(TNode* pParent, float x, float y, float w, float h);
	void	Buildtree(TNode*);
	bool    AddObject(XVector2 pos);
	TNode*  FindNode(TNode* pNode, XVector2 pos);
	void    Release();
	TNode*  FindPlayerNode(XVector2 pos);
public:
	void	Frame(float time);
public:
	TQuadtree();
	virtual ~TQuadtree();
};

