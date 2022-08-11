#include "TQuadtree.h"

std::queue<TNode*> g_Queue;
void   TQuadtree::DynamicObjectReset(TNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_ObjectDynamicList.clear();
    DynamicObjectReset(pNode->m_pChild[0]);
    DynamicObjectReset(pNode->m_pChild[1]);
    DynamicObjectReset(pNode->m_pChild[2]);
    DynamicObjectReset(pNode->m_pChild[3]);
}
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
// 1번 : 완전히 포함하는 노드에 추가하자.
// 2번 : 걸쳐만 있어도 노드에 추가하자.
void    TQuadtree::AddStaticObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectStaticList.push_back(pObj);
    }
}
void    TQuadtree::AddDynamicObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectDynamicList.push_back(pObj);
    }
}
TNode* TQuadtree::FindNode(TNode* pNode, TObject* pObj)
{
    do {
        for (int iNode = 0; iNode < 4; iNode++)
        {
            if (pNode->m_pChild[iNode] != nullptr)
            {
                bool bIn = TCollision::RectToInRect(
                    pNode->m_pChild[iNode]->m_rt,
                    pObj->m_rt);
                if (bIn > 0)
                {
                    g_Queue.push(pNode->m_pChild[iNode]);
                    break;
                }
            }
        }
        if (g_Queue.empty()) break;
        pNode = g_Queue.front();
        g_Queue.pop();
    } while (pNode);
    return pNode;
}

std::vector<TObject*> TQuadtree::Collision(TObject* pObj)
{
    std::vector<TObject*> list;
    GetCollisitionObject(this->m_pRootNode, pObj, list);
    return list;
};
void  TQuadtree::GetCollisitionObject(TNode* pNode,
    TObject* pSrcObject,
    std::vector<TObject*>& list)
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

