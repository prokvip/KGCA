#pragma once
#include "TNode.h"
#include "TObject2D.h"

class TSpacePartition
{
public:
	TNode* m_pRootNode;
public:
	virtual std::vector<TObject2D*> Collision(TObject2D* pObj)
	{
		std::vector<TObject2D*> ret;
		return ret;
	}
	virtual void    Create(float fWidth, float fHeight)
	{
		return;
	}
	virtual void    Create( float fWidth, 
							float fHeight,
							float fDepth)
	{
		return;
	}
public:
	virtual void    AddStaticObject(TObject2D* pObj);
	virtual void    AddDynamicObject(TObject2D* pObj);
	virtual void    DynamicObjectReset(TNode* pNode);
	virtual TNode*  FindNode(TNode* pNode, TObject2D* pObj);
	virtual bool    IsNodeInObject(TNode* pNode, TObject2D* pObj);
public:
	TSpacePartition();
	virtual ~TSpacePartition();
};

