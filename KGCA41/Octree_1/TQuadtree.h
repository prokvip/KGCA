#pragma once
#include "TSpacePartition.h"
static const int g_iMaxQuadtreeChild = 4;
class TQuadtree : public TSpacePartition
{
    TNode2D* m_pRootNode;
public:
    virtual void    Create(TVector2D vPos, TVector2D vSize) override;
    void            DynamicReset(TNode2D* pNode);
    virtual void    DynamicObjectReset();
protected:
    std::vector<TObject2D*> CollisionQuery(TObject2D* pObj);
    TNode2D* CreateNode(TNode2D* pParent, TVector2D vPos, TVector2D vSize);
    void    Buildtree(TNode2D* pNode);
    void    GetCollisitionObject(TNode2D* pNode,
        TObject2D* obj,
        std::vector<TObject2D*>& list);
public:    
    bool    IsNodeInObject(TNode2D* pNode, TObject2D* pObj);
public:
    TQuadtree();
    virtual ~TQuadtree();
};