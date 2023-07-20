#include "TOctree.h"

TNode* TOctree::CreateNode(TNode* pParent, float x,
    float y,
    float fWidth,
    float fHeight)
{
    TNode* pNode = new TNode(m_iNumNodeCounter++);
    pNode->SetParent(pParent);
    pNode->m_rt.Set(x, y, fWidth, fHeight);
    return pNode;
}
void TOctree::BuildTree(TNode* pNode)
{
    if (pNode->m_iDepth > 1)
    {
        return;
    }
    // 0, 0,  800, 600
    // v0-------vTC-------v1
    // |                  |
    // |                  |
    // vLC      vC       vRC
    // |                  |
    // |                  |
    // v3       vBC       v2

    TPoint vTC = { pNode->m_rt.m_Center.x, pNode->m_rt.m_Point[0].y };
    TPoint vRC = { pNode->m_rt.m_Point[1].x, pNode->m_rt.m_Center.y };
    TPoint vBC = { pNode->m_rt.m_Center.x, pNode->m_rt.m_Point[3].y };
    TPoint vLC = { pNode->m_rt.m_Point[0].x, pNode->m_rt.m_Center.y };

    TNode* pNewNode = CreateNode(pNode,
        pNode->m_rt.m_Point[0].x,
        pNode->m_rt.m_Point[0].y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    pNewNode = CreateNode(pNode,
        vTC.x,
        vTC.y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    pNewNode = CreateNode(pNode,
        pNode->m_rt.m_Center.x,
        pNode->m_rt.m_Center.y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    pNewNode = CreateNode(pNode,
        vLC.x,
        vLC.y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);


    std::cout << "[" << pNode->m_iDepth << "]"
        << pNode->m_rt.m_fx << ","
        << pNode->m_rt.m_fy << ","
        << pNode->m_rt.m_fWidth << ","
        << pNode->m_rt.m_fHeight << std::endl;

    for (int i = 0; i < pNode->m_pChild.size(); i++)
    {
        BuildTree(pNode->m_pChild[i]);
    }
}
void TOctree::BuildOctTree(
    float x, float y, float w, float h)
{
    m_pRootNode = CreateNode(nullptr, x, y, w, h);
    BuildTree(m_pRootNode);
}
