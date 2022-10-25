#include "TQuadtree.h"
#include "TCollision.h"
bool TQuadtree::Create(TMap* pMap, int iMaxDepth)
{
    m_pMap = pMap;
    m_iMaxDepth = iMaxDepth;
    m_pRootNode = new TNode(nullptr, 
        0, 
        pMap->m_dwNumRows-1,
        pMap->m_dwNumRows * (pMap->m_dwNumColumns - 1),
        pMap->m_dwNumRows* pMap->m_dwNumColumns-1 );
    BuildTree(m_pRootNode);
    return true;
}
TNode* TQuadtree::FindNode(TNode* pNode, TObject3D* pObj)
{
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            /*if (TCollision::RectToInRect(pNode->m_pChild[i]->m_rt, pObj->m_rt))
            {
                pNode = FindNode(pNode->m_pChild[i], pObj);
                break;
            }*/
        }
    }
    return pNode;
}
bool TQuadtree::AddObject(TObject3D* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_pDynamicObjectlist.push_back(pObj);
        return true;
    }
    return false;
}
void TQuadtree::Reset(TNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_pDynamicObjectlist.clear();
    Reset(pNode->m_pChild[0]);
    Reset(pNode->m_pChild[1]);
    Reset(pNode->m_pChild[2]);
    Reset(pNode->m_pChild[3]);
}
void TQuadtree::BuildTree(TNode* pNode)
{
    if (pNode == nullptr) return;
    if (IsSubDivide(pNode) == false)
    {
        pNode->m_bLeaf = true;
        return;
    }
    pNode->CreateChildNode(pNode);

    BuildTree(pNode->m_pChild[0]);
    BuildTree(pNode->m_pChild[1]);
    BuildTree(pNode->m_pChild[2]);
    BuildTree(pNode->m_pChild[3]);
}

bool TQuadtree::IsSubDivide(TNode* pNode)
{
    if ((pNode->m_Corner[1] - pNode->m_Corner[0] )==1) return true;
    if (pNode->m_iDepth < m_iMaxDepth) return true;
    return false;
}
