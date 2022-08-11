#pragma once
#include "TNode.h"
#include "TObject.h"


class TQuadtree
{
public:
    TNode* m_pRootNode;
public:
    std::vector<TObject*> Collision(TObject* pObj);
    void    Create(float fWidth, float fHeight);
    TNode* CreateNode(TNode* pParent,
        float fPosX, float fPosY,
        float fWidth, float fHeight);
    void    Buildtree(TNode* pNode);
    void    AddStaticObject(TObject* pObj);
    void    AddDynamicObject(TObject* pObj);
    TNode* FindNode(TNode* pNode, TObject* pObj);
    void    GetCollisitionObject(TNode* pNode,
        TObject* obj,
        std::vector<TObject*>& list);

    void   DynamicObjectReset(TNode* pNode);
    ~TQuadtree()
    {
        delete m_pRootNode;
    }
};