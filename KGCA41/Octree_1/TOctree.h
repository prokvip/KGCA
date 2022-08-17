#pragma once
#include "TSpacePartition.h"

static const int g_iMaxOctreeChild = 8;
class TOctree :  public TSpacePartition
{
	TNode*  m_pRootNode;
public:
	void     DynamicReset(TNode* pNode);
	virtual void    DynamicObjectReset();

	void    Create(TVector vPos, TVector vSize) override;
	bool    IsNodeInObject(TNode* pNode, TObject* pObj);
	bool    IsCollision(TObject* pDest, TObject* pSrc);
	bool	IsCollision(TNode* pNode, TObject* pSrc);
public:
	TNode*	CreateNode(TNode* pParent,TVector vPos,TVector vSize);
	void	Buildtree(TNode* pNode);	
	void    AddStaticObject(TObject* pObj);
    void    AddDynamicObject(TObject* pObj);
    TNode*  FindNode(TNode* pNode, TObject* pObj);
    std::vector<TObject*> CollisionQuery(TObject* pObj);
    void   GetCollisitionObject(TNode* pNode,
				TObject* pSrcObject,
				std::vector<TObject*>& list);
};

