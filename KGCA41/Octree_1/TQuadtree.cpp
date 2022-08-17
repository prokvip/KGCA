#include "TQuadtree.h"
void   TQuadtree::DynamicReset(TNode2D* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_ObjectDynamicList2D.clear();
    for (int iChild = 0; iChild < pNode->m_pChild2D.size(); iChild++)
    {
        DynamicReset(pNode->m_pChild2D[iChild]);
    }    
}
void    TQuadtree::DynamicObjectReset()
{
    DynamicReset(m_pRootNode);
}
void TQuadtree::Create(TVector2D vPos, TVector2D vSize)
{
    m_pRootNode = CreateNode(nullptr, vPos, vSize);
    Buildtree(m_pRootNode);
}
void TQuadtree::Buildtree(TNode2D* pNode)
{
    if (pNode->m_iDepth >= 2) return;
    if (pNode == nullptr) return;

    TVector2D p, s;
    p.x = pNode->m_rt.x1;
    p.y = pNode->m_rt.y1;
    s.x = pNode->m_rt.w / 2.0f;
    s.y = pNode->m_rt.h / 2.0f;
    pNode->m_pChild2D[0] = CreateNode(pNode, p, s);
    p.x = pNode->m_rt.x1 + s.x;
    p.y = pNode->m_rt.y1;
    pNode->m_pChild2D[1] = CreateNode(pNode, p, s);
    p.x = pNode->m_rt.x1;
    p.y = pNode->m_rt.y1 + s.y;
    pNode->m_pChild2D[2] = CreateNode(pNode, p, s);
    p.x = pNode->m_rt.x1 + s.x;
    p.y = pNode->m_rt.y1 + s.y;
    pNode->m_pChild2D[3] = CreateNode(pNode, p, s);

    for (int iChild = 0; iChild < g_iMaxQuadtreeChild; iChild++)
    {
        Buildtree(pNode->m_pChild2D[iChild]);
    }
}
TNode2D* TQuadtree::CreateNode(
    TNode2D* pParent,
    TVector2D vPos,
    TVector2D vSize)
{
    TNode2D* pNode = new TNode2D(pParent, vPos, vSize);
    pNode->m_pChild2D.resize(g_iMaxQuadtreeChild);
    return pNode;
}


std::vector<TObject2D*> TQuadtree::CollisionQuery(TObject2D* pObj)
{
    std::vector<TObject2D*> list;
    GetCollisitionObject(this->m_pRootNode, pObj, list);
    return list;
};
void  TQuadtree::GetCollisitionObject(TNode2D* pNode,
    TObject2D* pSrcObject,
    std::vector<TObject2D*>& list)
{
    if (pNode == nullptr) return;
    for (int iObj = 0; iObj < pNode->m_ObjectStaticList2D.size(); iObj++)
    {
        if (TCollision::RectToRect(
            pNode->m_ObjectStaticList2D[iObj]->m_rt,
            pSrcObject->m_rt))
        {
            list.push_back(pNode->m_ObjectStaticList2D[iObj]);
        }
    }
    for (int iObj = 0; iObj < pNode->m_ObjectDynamicList2D.size(); iObj++)
    {
        if (TCollision::CircleToCircle(
            pNode->m_ObjectDynamicList2D[iObj]->m_Circle,
            pSrcObject->m_Circle))
        {
            list.push_back(pNode->m_ObjectDynamicList2D[iObj]);
        }
    }
    if (pNode->m_pChild2D[0] != nullptr)
    {
        for (int iChild = 0; iChild < 4; iChild++)
        {
            if (TCollision::RectToRect(
                pNode->m_pChild2D[iChild]->m_rt,
                pSrcObject->m_rt))
            {
                GetCollisitionObject(pNode->m_pChild2D[iChild], 
                    pSrcObject, list);
            }
        }
    }
}

bool   TQuadtree::IsNodeInObject(TNode2D* pNode, TObject2D* pObj)
{
    bool bIn = TCollision::RectToInRect(pNode->m_rt,   pObj->m_rt);
    return bIn;
}

TQuadtree::TQuadtree()
{}
TQuadtree::~TQuadtree()
{}