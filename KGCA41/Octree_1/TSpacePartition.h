#pragma once
#include "TNode.h"
//#include "TObject.h"

class TSpacePartition
{
public:
	TNode* m_pRootNode;
public:
	//virtual std::vector<TObject*> Collision(TObject* pObj)
	//{
	//	std::vector<TObject2D*> ret;
	//	return ret;
	//}
	virtual void    Create( float fWidth, 
							float fHeight,
							float fDepth)
	{
		return;
	}
//public:
//	virtual void    AddStaticObject(TObject* pObj);
//	virtual void    AddDynamicObject(TObject* pObj);
//	virtual void    DynamicObjectReset(TNode* pNode);
//	virtual TNode*  FindNode(TNode* pNode, TObject2D* pObj);
//	virtual bool    IsNodeInObject(TNode* pNode, TObject2D* pObj);
public:
	TSpacePartition();
	virtual ~TSpacePartition();
};

