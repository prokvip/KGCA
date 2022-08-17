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
void    TSpacePartition::AddStaticObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectStaticList.push_back(pObj);
    }
}
void    TSpacePartition::AddDynamicObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectDynamicList.push_back(pObj);
    }
}

TNode* TSpacePartition::FindNode(TNode* pNode, TObject* pObj)
{
    std::queue<TNode*> g_Queue;
    do {
        for (int iNode = 0; iNode < pNode->m_pChild.size(); iNode++)
        {
            if (pNode->m_pChild[iNode] != nullptr)
            {
                bool bIn = IsNodeInObject(pNode->m_pChild[iNode],pObj);
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
std::vector<TObject*> TSpacePartition::CollisionQuery(TObject* pObj)
{
    std::vector<TObject*> list;
    GetCollisitionObject(this->m_pRootNode, pObj, list);
    return list;
};
void  TSpacePartition::GetCollisitionObject(TNode* pNode,
    TObject* pSrcObject,
    std::vector<TObject*>& list)
{
    if (pNode == nullptr) return;
    for (int iObj = 0; iObj < pNode->m_ObjectStaticList.size(); iObj++)
    {
        if (IsCollision( pNode->m_ObjectStaticList[iObj],pSrcObject))
        {
            list.push_back(pNode->m_ObjectStaticList[iObj]);
        }       
    }
    for (int iObj = 0; iObj < pNode->m_ObjectDynamicList.size(); iObj++)
    {
        if (IsCollision(pNode->m_ObjectDynamicList[iObj],pSrcObject))
        {
            list.push_back(pNode->m_ObjectDynamicList[iObj]);
        }        
    }
    if (pNode->m_pChild[0] != nullptr)
    {
        for (int iChild = 0; iChild < pNode->m_pChild.size(); iChild++)
        {
            if (IsCollision(pNode->m_pChild[iChild],pSrcObject))
            {
                GetCollisitionObject(pNode->m_pChild[iChild], pSrcObject, list);
            }          
        }
    }
}
bool   TSpacePartition::IsNodeInObject(TNode* pNode, TObject* pObj)
{
    return true;
}
bool   TSpacePartition::IsCollision(TObject* pDest, TObject* pSrc)
{
    return true;
}
bool   TSpacePartition::IsCollision(TNode* pNode, TObject* pSrc)
{
    return true;
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