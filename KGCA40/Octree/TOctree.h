#pragma once
#include "TNode.h"
#include <queue>
#include "TCollision.h"
#include "TObject.h"
class TOctree
{
public:
	TNode*					m_pRootNode;
	XVector3				m_vSize;
	std::queue<TNode*>		m_Queue;
	TObject*				m_pPlayer;
	std::map<int,TObject*>  m_ObjectList;	
public:
	bool    Init(float fMaxX, float fMaxY, float fMaxZ);
	TNode*  CreateNode(TNode* pParent, XVector3 vPivot, XVector3 vSize);
	int		CheckRect(TNode* pNode, TObject* pObj);
	void	Buildtree(TNode*);
	bool    AddObject(TObject* obj);
	TNode*  FindNode(TNode* pNode, TObject* pObj);
	void    Release();
public:
	void	Frame(float time);
public:
	TOctree();
	virtual ~TOctree();
};

