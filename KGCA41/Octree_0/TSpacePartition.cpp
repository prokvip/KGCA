#include "TSpacePartition.h"
void   TSpacePartition::DynamicObjectReset(TNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_ObjectDynamicList.clear();
    for (int iChild = 0; iChild < pNode->m_pChild.size(); iChild++)
    {
        DynamicObjectReset(pNode->m_pChild[iChild]);
    }
}
// 1번 : 완전히 포함하는 노드에 추가하자.
// 2번 : 걸쳐만 있어도 노드에 추가하자.
void    TSpacePartition::AddStaticObject(TObject2D* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectStaticList.push_back(pObj);
    }
}
void    TSpacePartition::AddDynamicObject(TObject2D* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectDynamicList.push_back(pObj);
    }
}
bool   TSpacePartition::IsNodeInObject(TNode* pNode, TObject2D* pObj)
{
    return true;
}
TNode* TSpacePartition::FindNode(TNode* pNode, TObject2D* pObj)
{
    std::queue<TNode*> g_Queue;
    do {
        for (int iNode = 0; iNode < pNode->m_pChild.size(); iNode++)
        {
            if (pNode->m_pChild[iNode] != nullptr)
            {
                bool bIn = IsNodeInObject(
                    pNode->m_pChild[iNode],
                    pObj);
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
TSpacePartition::TSpacePartition()
{
	m_pRootNode = nullptr;
}
TSpacePartition::~TSpacePartition()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}