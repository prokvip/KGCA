#pragma once
#include "TNode.h"
#include "TObject.h"

class TSpacePartition
{
public:
	TNode* m_pRootNode;
public:
	//virtual std::vector<TObject*> Collision(TObject* pObj)
	//{
	//	std::vector<TObject*> ret;
	//	return ret;
	//}
	virtual void    Create(TVector vPos, TVector vSize)=0;
public:
	virtual void    AddStaticObject(TObject* pObj);
	virtual void    AddDynamicObject(TObject* pObj);
	virtual void    DynamicObjectReset(TNode* pNode);
	virtual TNode*  FindNode(TNode* pNode, TObject* pObj);
	virtual bool    IsNodeInObject(TNode* pNode, TObject* pObj);
public:
	TSpacePartition();
	virtual ~TSpacePartition();
};

