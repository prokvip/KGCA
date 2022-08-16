#pragma once
#include <windows.h>
#include <map>
#include <queue>
#include "TObject2D.h"
class TNode
{
public:
    TRect   m_rt;
    int     m_iDepth;
    std::vector<TObject2D*>     m_ObjectStaticList;
    std::vector<TObject2D*>     m_ObjectDynamicList;
    std::vector<TNode*>         m_pChild;    
    TNode* m_pParent;
public:
    TNode(TNode* pParent, float x, float y, float w, float h);
    ~TNode();
};
