#pragma once
#include "TNode.h"
#include "TObject.h"

class TSpacePartition
{
public:
	TNode* m_pRootNode;
public:
	virtual std::vector<TObject*> TSpacePartition::CollisionQuery(TObject* pObj);
	virtual void    GetCollisitionObject(TNode* pNode,
										TObject* pSrcObject,
										std::vector<TObject*>& list);
	virtual void    Create(TVector vPos, TVector vSize)=0;
	virtual void    AddStaticObject(TObject* pObj);
	virtual void    AddDynamicObject(TObject* pObj);
	virtual void    DynamicObjectReset(TNode* pNode);
	virtual TNode*  FindNode(TNode* pNode, TObject* pObj);
	virtual bool    IsNodeInObject(TNode* pNode, TObject* pObj);
	virtual bool    IsCollision(TObject* pDest, TObject* pSrc);
	virtual bool	IsCollision(TNode* pNode, TObject* pSrc);
public:
	TSpacePartition();
	virtual ~TSpacePartition();
};

