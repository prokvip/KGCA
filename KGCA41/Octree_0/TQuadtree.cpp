#include "TQuadtree.h"
void TQuadtree::Create(
    float fWidth,
    float fHeight)
{
    m_pRootNode = CreateNode(nullptr,
        0, 0, fWidth, fHeight);
    Buildtree(m_pRootNode);
}
void TQuadtree::Buildtree(TNode* pNode)
{
    if (pNode->m_iDepth >= 2) return;
    if (pNode == nullptr) return;

    float x, y, w, h;
    x = pNode->m_rt.x1;
    y = pNode->m_rt.y1;
    w = pNode->m_rt.w / 2.0f;
    h = pNode->m_rt.h / 2.0f;
    pNode->m_pChild[0] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x1 + w;
    y = pNode->m_rt.y1;
    pNode->m_pChild[1] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x1;
    y = pNode->m_rt.y1 + h;
    pNode->m_pChild[2] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x1 + w;
    y = pNode->m_rt.y1 + h;
    pNode->m_pChild[3] = CreateNode(pNode, x, y, w, h);

    for (int iChild = 0; iChild < 4; iChild++)
    {
        Buildtree(pNode->m_pChild[iChild]);
    }
}
TNode* TQuadtree::CreateNode(
    TNode* pParent,
    float x, float y,
    float w, float h)
{
    TNode* pNode = new TNode(pParent, x, y, w, h);
    return pNode;
}



std::vector<TObject2D*> TQuadtree::Collision(TObject2D* pObj)
{
    std::vector<TObject2D*> list;
    GetCollisitionObject(this->m_pRootNode, pObj, list);
    return list;
};
void  TQuadtree::GetCollisitionObject(TNode* pNode,
    TObject2D* pSrcObject,
    std::vector<TObject2D*>& list)
{
    if (pNode == nullptr) return;
    for (int iObj = 0; iObj < pNode->m_ObjectStaticList.size(); iObj++)
    {
        if (TCollision::RectToRect(
            pNode->m_ObjectStaticList[iObj]->m_rt,
            pSrcObject->m_rt))
        {
            list.push_back(pNode->m_ObjectStaticList[iObj]);
        }
    }
    for (int iObj = 0; iObj < pNode->m_ObjectDynamicList.size(); iObj++)
    {
        if (TCollision::CircleToCircle(
            pNode->m_ObjectDynamicList[iObj]->m_Circle,
            pSrcObject->m_Circle))
        {
            list.push_back(pNode->m_ObjectDynamicList[iObj]);
        }
    }
    if (pNode->m_pChild[0] != nullptr)
    {
        for (int iChild = 0; iChild < 4; iChild++)
        {
            if (TCollision::RectToRect(
                pNode->m_pChild[iChild]->m_rt,
                pSrcObject->m_rt))
            {
                GetCollisitionObject(pNode->m_pChild[iChild], pSrcObject, list);
            }
        }
    }
}

bool   TQuadtree::IsNodeInObject(TNode* pNode, TObject2D* pObj)
{
    bool bIn = TCollision::RectToInRect(
        pNode->m_rt,
        pObj->m_rt);
    return bIn;
}

TQuadtree::TQuadtree()
{}
TQuadtree::~TQuadtree()
{}