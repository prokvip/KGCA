#include "TQuadtree.h"
#include "TCollision.h"
bool TQuadtree::Create(TRect rt, int iMaxDepth)
{
    m_iMaxDepth = iMaxDepth;
    m_pRootNode = new TNode(nullptr, rt);
    BuildTree(m_pRootNode);
    return true;
}
TNode* TQuadtree::FindNode(TNode* pNode, TObject* pObj)
{   
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            if (TCollision::RectToInRect(pNode->m_pChild[i]->m_rt, pObj->m_rt))
            {
                pNode = FindNode(pNode->m_pChild[i], pObj);
                break;
            }
        }
    }
    return pNode;
}
bool TQuadtree::AddObject(TObject* pObj)
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
    if (IsSubDivide(pNode)== false)
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
    if (pNode->m_iDepth < m_iMaxDepth) return true;
    return false;
}
