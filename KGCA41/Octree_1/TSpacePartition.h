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
public:
	virtual void    DynamicObjectReset();
public:
	virtual std::vector<TObject2D*> CollisionQuery(TObject2D* pObj);
	virtual void    Create(TVector2D vPos, TVector2D vSize);
	virtual void    AddStaticObject2D(TObject2D* pObj);
	virtual void    AddDynamicObject2D(TObject2D* pObj);
public:
	virtual std::vector<TObject*>   CollisionQuery(TObject* pObj);		
	virtual void    Create(TVector vPos, TVector vSize);	
	virtual void    AddStaticObject(TObject* pObj);
	virtual void    AddDynamicObject(TObject* pObj);
protected:
	virtual TNode2D*	FindNode(TNode2D* pNode, TObject2D* pObj);
	virtual TNode*		FindNode(TNode* pNode, TObject* pObj);
public:
	TSpacePartition();
	virtual ~TSpacePartition();
};

