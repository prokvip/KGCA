#pragma once
#include <windows.h>
#include <map>
#include <queue>
#include <iostream>
#include "TCollision.h"
#include "TObject.h"
class TNode2D
{
public:
    TRect   m_rt;
    int     m_iDepth;
    std::vector<TObject2D*>     m_ObjectStaticList2D;
    std::vector<TObject2D*>     m_ObjectDynamicList2D;
    std::vector<TNode2D*>       m_pChild2D;
    TNode2D* m_pParent;
public:
    TNode2D();
    TNode2D(TNode2D* pParent, TVector2D p, TVector2D s);
    ~TNode2D();
};
class TNode : public TNode2D
{
public:
    TBox   m_Box;
    int    m_iDepth;
    std::vector<TObject*>  m_ObjectStaticList;
    std::vector<TObject*>  m_ObjectDynamicList;
    std::vector<TNode*>    m_pChild;    
    TNode* m_pParent;
public:
    TNode();
    TNode(TNode* pParent,
        TVector vPos,
        TVector vSize);
    ~TNode();
};
