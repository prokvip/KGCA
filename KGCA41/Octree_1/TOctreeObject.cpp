#include "TOctree.h"

TNode* TOctree::FindNode(TNode* pNode, TObject3D* obj)
{
    if (pNode == nullptr) return nullptr;
    do
    {
        for (int i = 0; i < pNode->m_pChild.size(); i++)
        {
            if (pNode->m_pChild[i] != nullptr)
            {
                //if (TCollision::RectToPoint(pNode->m_pChild[i]->m_tRT,
                //obj->m_Position))
                //todo
               /* if (TCollision::RectToRect(
                    pNode->m_pChild[i]->m_tBox,
                    obj->m_tBox))
                {
                    g_Queue.push(pNode->m_pChild[i]);
                    break;
                }*/
            }
        }
        if (g_Queue.empty()) break;
        pNode = g_Queue.front();
        g_Queue.pop();
    }
    while (pNode);
    return pNode;
}
TNode* TOctree::StaticAddObject(TObject3D* obj)
{
    TNode* pFindNode = FindNode(m_pRootNode, obj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_StaticObjectList3D.push_back(obj);
        return pFindNode;
    }
    return nullptr;
}
TNode* TOctree::DynamicAddObject(TObject3D* obj)
{
    TNode* pFindNode = FindNode(m_pRootNode, obj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_DynamicObjectList3D.push_back(obj);
        m_DynamicObjectNodeList.insert(pFindNode);
        return pFindNode;
    }
    return nullptr;
}
