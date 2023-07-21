#pragma once
#include "TObject.h"

class TNode
{
public:
    int                     m_iIndex = -1;
    int                     m_iDepth = 0;
    TRect                   m_tRT;
    TBox                    m_tBox;
    TNode*                  m_pParent = nullptr;
    std::vector<TNode*>     m_pChild;
    std::vector<TObject2D*>   m_StaticObjectList;
    std::vector<TObject2D*>   m_DynamicObjectList;
    std::vector<TObject3D*>   m_StaticObjectList3D;
    std::vector<TObject3D*>   m_DynamicObjectList3D;
public:    
    void   SetParent(TNode* pParent);
public:
    TNode(int id);
    TNode();
    virtual ~TNode();
};

struct TSpaceData
{
    TPoint2 p2;
    TPoint3 p3;
    float  w;
    float  h;
    float  z;
};