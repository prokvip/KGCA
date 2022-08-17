#pragma once
#include "TSpacePartition.h"

static const int g_iMaxOctreeChild = 8;
class TOctree :  public TSpacePartition
{
public:
	void    Create(TVector vPos, TVector vSize) override;
	bool    IsNodeInObject(TNode* pNode, TObject* pObj)override;
	bool    IsCollision(TObject* pDest, TObject* pSrc)override;
	bool	IsCollision(TNode* pNode, TObject* pSrc)override;
public:
	TNode*	CreateNode(TNode* pParent,TVector vPos,TVector vSize);
	void	Buildtree(TNode* pNode);	
};

