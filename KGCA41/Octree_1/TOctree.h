#pragma once
#include "TSpacePartition.h"

static const int g_iMaxOctreeChild = 8;
class TOctree :  public TSpacePartition
{
public:
	void    Create(TVector vPos, TVector vSize) override;
	TNode*	CreateNode(TNode* pParent,TVector vPos,TVector vSize);
	void	Buildtree(TNode* pNode);
	bool    IsNodeInObject(TNode* pNode, TObject* pObj);
};

