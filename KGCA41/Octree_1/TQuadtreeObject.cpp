#include "TQuadtree.h"

TNode* TQuadtree::FindNode(TNode* pNode, TObject* obj)
{
    if (pNode == nullptr) return nullptr;
    do
    {
        for (int i = 0; i < pNode->m_pChild.size(); i++)
        {
            if (pNode->m_pChild[i] != nullptr)
            {
                //if (TCollision::RectToPoint(pNode->m_pChild[i]->m_rt,
                //obj->m_Position))
                if (TCollision::RectToRect(
                    pNode->m_pChild[i]->m_rt,
                    obj->m_rt))
                {
                    g_Queue.push(pNode->m_pChild[i]);
                    break;
                }
            }
        }
        if (g_Queue.empty()) break;
        pNode = g_Queue.front();
        g_Queue.pop();
    }
    while (pNode);
    return pNode;
}
TNode* TQuadtree::StaticAddObject(TObject* obj)
{
    TNode* pFindNode = FindNode(m_pRootNode, obj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_StaticObjectList.push_back(obj);
        return pFindNode;
    }
    return nullptr;
}
TNode* TQuadtree::DynamicAddObject(TObject* obj)
{
    TNode* pFindNode = FindNode(m_pRootNode, obj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_DynamicObjectList.push_back(obj);
        m_DynamicObjectNodeList.insert(pFindNode);
        return pFindNode;
    }
    return nullptr;
}
