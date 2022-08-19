#pragma once
#include "TSpacePartition.h"
static const int g_iMaxQuadtreeChild = 4;
class TQuadtree : public TSpacePartition
{
public:
    void    DynamicObjectReset()override;;
    void    Create(TVector2D vPos, TVector2D vSize) override;
    void    AddStaticObject(TBaseObject* pObj)override;
    void    AddDynamicObject(TBaseObject* pObj)override;
    TNode*  FindNode(TNode* pNode, TBaseObject* pObj)override;
    std::vector<TBaseObject*> CollisionQuery(TBaseObject* pObj)override;
protected: 
    void    DynamicReset(TNode* pNode);    
    TNode*  CreateNode(TNode* pParent, TVector2D vPos, TVector2D vSize);
    void    Buildtree(TNode* pNode);
    void    GetCollisitionObject(TNode* pNode,    TBaseObject* obj,
                                    std::vector<TBaseObject*>& list);    
    bool    IsNodeInObject(TNode* pNode, TBaseObject* pObj);
public:
    TQuadtree();
    virtual ~TQuadtree();
};