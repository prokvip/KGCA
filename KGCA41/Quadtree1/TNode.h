#pragma once
#include "TObject.h"
class TNode
{
public:
    TRect  m_rt;
    int    m_iDepth = 0;
    TNode* m_pParent = nullptr;
    std::vector<TNode*> m_pChild;
    std::vector<TObject*> m_ObjectList;
public:    
    void   SetParent(TNode* pParent);
    TNode(TNode* pParent, 
        float x,
        float y, 
        float fWidth,
        float fHeight) : m_pParent(pParent)
    {
        m_rt.Set(x, y, fWidth, fHeight);
        SetParent(pParent);
    }
    ~TNode()
    {
        if (m_pChild.size() != 0)
        {
            delete m_pChild[0];
            delete m_pChild[1];
            delete m_pChild[2];
            delete m_pChild[3];
        }
    }
};

