#pragma once
#include "TNode.h"
class TQuadtree
{    
    std::queue<TNode*> g_Queue;    
    TNode* g_pRootNode = nullptr;
public:
    TNode* GetRootNode();
public:
    void Buildtree(TNode* pNode);
    void BuildQuadtree();
    TNode* CreateNode(TNode* pParent, float x,
        float y,
        float fWidth,
        float fHeight);

    TNode* AddObject(TObject* obj);
    TNode* FindNode(TNode* pNode, TObject* obj);
public:
    void PreOrder(TNode* pNode);
    void PostOrder(TNode* pNode);
    void LevelOrder(TNode* pNode);
    ~TQuadtree()
    {
        delete g_pRootNode;
    }
};

