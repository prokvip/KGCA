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
    TBox    m_Box;
    int     m_iDepth;
    TRect   m_rt;
    std::vector<TBaseObject*>   m_ObjectStaticList;
    std::vector<TBaseObject*>   m_ObjectDynamicList;
    std::vector<TNode*>         m_pChild;    
    TNode*                      m_pParent;
public:
    TNode();
    TNode(TNode* pParent,TVector vPos,TVector vSize);
    TNode(TNode* pParent, TVector2D p, TVector2D s);
    ~TNode();
};
