#pragma once
#include "TSpacePartition.h"
class TQuadtree : public TSpacePartition
{
public:
    std::vector<TObject2D*> Collision(TObject2D* pObj) override;
    void    Create(float fWidth, float fHeight)override;

    TNode* CreateNode(TNode* pParent,
        float fPosX, float fPosY,
        float fWidth, float fHeight);
    void    Buildtree(TNode* pNode);
    void    GetCollisitionObject(TNode* pNode,
        TObject2D* obj,
        std::vector<TObject2D*>& list);
public:
    bool    IsNodeInObject(TNode* pNode, TObject2D* pObj)override;
public:
    TQuadtree();
    virtual ~TQuadtree();
};