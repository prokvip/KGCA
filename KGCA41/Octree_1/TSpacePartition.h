#pragma once
#include "TNode.h"
#include "TObject.h"
enum SP_TYPE
{
	T_QUADTREE,
	T_OCTREE,
	T_BSP,
};
class TSpacePartition
{
private:
	SP_TYPE     m_spType= T_OCTREE;
protected:
	TNode*		m_pRootNode;
public:
	virtual void    DynamicObjectReset();
public:
	virtual void    Create(TVector2D vPos, TVector2D vSize);
	virtual void    Create(TVector vPos, TVector vSize);
public:
	virtual std::vector<TBaseObject*>   CollisionQuery(TBaseObject* pObj);		
	virtual void    AddStaticObject(TBaseObject* pObj);
	virtual void    AddDynamicObject(TBaseObject* pObj);
protected:
	virtual TNode*	FindNode(TNode* pNode, TBaseObject* pObj);
public:
	TSpacePartition();
	virtual ~TSpacePartition();
};

