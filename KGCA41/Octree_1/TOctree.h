#pragma once
#include "TSpacePartition.h"
class TOctree :  public TSpacePartition
{
public:
	void    Create(float fWidth,float fHeight,float fDepth) override;
	TNode*	CreateNode(TNode* pParent,
						float x, float y, float z,
						float w, float h, float d);
	void	Buildtree(TNode* pNode);
};

