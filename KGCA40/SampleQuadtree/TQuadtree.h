#pragma once
#include "TNode.h"
#include <queue>

class TQuadtree
{
public:
	TNode*		m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	std::queue<TNode*>  m_Queue;
	TMap*		m_pMap;
public:
	void    Build(TMap* pMap);
	bool    Init();
	TNode*  CreateNode(TNode* pParent, float x, float y, float w, float h);
	void	Buildtree(TNode*);
	bool    AddObject(TVector2 pos);
	TNode*  FindNode(TNode* pNode, TVector2 pos);
	void    Release();
	TNode*  FindPlayerNode(TVector2 pos);
public:
	void	Frame(float time);
public:
	TQuadtree();
	virtual ~TQuadtree();
};

