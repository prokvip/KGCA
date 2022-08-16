#pragma once
#include <windows.h>
#include <map>
#include <queue>
#include <iostream>
#include "TCollision.h"
#include "TObject.h"
class TNode
{
public:
    TBox   m_Box;
    int    m_iDepth;
    std::vector<TObject*>  m_ObjectStaticList;
    std::vector<TObject*>  m_ObjectDynamicList;
    std::vector<TNode*>    m_pChild;    
    TNode* m_pParent;
public:
    TNode(TNode* pParent, 
          TVector vPos,
          TVector vSize)
    {
        m_Box.Set(vPos, vSize);
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
        for (int iChild = 0; iChild < m_pChild.size(); iChild++)
        {
            delete m_pChild[iChild];
        }
    }
};
