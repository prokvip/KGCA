#pragma once
#include "TObject.h"

extern int   g_iCounter;

class TNode
{
public:
    int    m_iIndex = -1;
    int    m_iDepth = 0;
    TRect  m_rt;
    TNode* m_pParent = nullptr;
    std::vector<TNode*> m_pChild;
    std::vector<TObject*> m_StaticObjectList;
    std::vector<TObject*> m_DynamicObjectList;

    virtual TNode* CreateNode()
    {
        return new TNode(g_iCounter++);
    };
    virtual void   CreateChildNode()
    {
    };
    virtual void   SetParent(TNode* pParent)
    {
        if (pParent == nullptr) return;
        m_pParent = pParent;
        m_iDepth = pParent->m_iDepth + 1;
    }
    TNode(int id) : m_iIndex(id)    {    }
    TNode() {}
    TNode(TNode* pParent,float x,float y,float fWidth,float fHeight);
    virtual ~TNode() {}
};

