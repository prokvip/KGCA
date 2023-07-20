#pragma once
#include "TObject.h"

class TNode
{
public:
    int                     m_iIndex = -1;
    int                     m_iDepth = 0;
    TRect                   m_rt;
    TNode*                  m_pParent = nullptr;
    std::vector<TNode*>     m_pChild;
    std::vector<TObject*>   m_StaticObjectList;
    std::vector<TObject*>   m_DynamicObjectList;
public:    
    void   SetParent(TNode* pParent);
public:
    TNode(int id);
    TNode();
    virtual ~TNode();
};

