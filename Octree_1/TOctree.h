#pragma once
#include "TSpacePartition.h"

static const int g_iMaxOctreeChild = 8;
class TOctree :  public TSpacePartition
{	
public:
	void    DynamicObjectReset()override;;	
	void    Create(TVector vPos, TVector vSize) override;
	void    AddStaticObject(TBaseObject* pObj)override;
	void    AddDynamicObject(TBaseObject* pObj)override;
	TNode*  FindNode(TNode* pNode, TBaseObject* pObj)override;
	std::vector<TBaseObject*> CollisionQuery(TBaseObject* pObj)override;
public:
	void    DynamicReset(TNode* pNode);	
	bool    IsNodeInObject(TNode* pNode, TBaseObject* pObj);
	bool    IsCollision(TBaseObject* pDest, TBaseObject* pSrc);
	bool	IsCollision(TNode* pNode, TBaseObject* pSrc);
	TNode*	CreateNode(TNode* pParent,TVector vPos,TVector vSize);
	void	Buildtree(TNode* pNode);	
    void    GetCollisitionObject(TNode* pNode,TBaseObject* pSrcObject,
								 std::vector<TBaseObject*>& list);
};

