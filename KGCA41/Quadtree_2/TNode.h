#pragma once
#include <windows.h>
#include <map>
#include <queue>
#include "TObject.h"
class TNode
{
public:
    TRect m_rt;
    int    m_iDepth;
    std::vector<TObject*>  m_ObjectStaticList;
    std::vector<TObject*>  m_ObjectDynamicList;
    TNode* m_pChild[4];
    TNode* m_pParent;
public:
    TNode(TNode* pParent, float x, float y, float w, float h)
    {
        m_rt.Set(x, y, w, h);
        m_iDepth = 0;
        m_pParent = nullptr;
        if (pParent != nullptr)
        {
            m_pParent = pParent;
            m_iDepth = pParent->m_iDepth + 1;
        }
    }
    ~TNode()
    {
        delete m_pChild[0];
        delete m_pChild[1];
        delete m_pChild[2];
        delete m_pChild[3];
    }
};
