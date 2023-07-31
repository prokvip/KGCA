#include "TQuadtree.h"
TNode* TQuadtree::CreateNode(TNode* pParent, float x,
    float y,
    float fWidth,
    float fHeight)
{
    TNode* pNode = new TNode(m_iNumNodeCounter++);
    pNode->SetParent(pParent);
    pNode->m_tRT.Set(x, y, fWidth, fHeight);
    return pNode;
}
TNode* TQuadtree::CreateNode(TNode* pParent, TSpaceData data)
{
    TNode* pNode = new TNode(m_iNumNodeCounter++);
    pNode->SetParent(pParent);
    pNode->m_tRT.Set(data.p2.x, data.p2.y, data.w, data.h);
    return pNode;
}
void TQuadtree::BuildTree(TNode* pNode)
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

    TVector2 vTC = { pNode->m_tRT.m_Center.x, pNode->m_tRT.m_Point[0].y };
    TVector2 vRC = { pNode->m_tRT.m_Point[1].x, pNode->m_tRT.m_Center.y };
    TVector2 vBC = { pNode->m_tRT.m_Center.x, pNode->m_tRT.m_Point[3].y };
    TVector2 vLC = { pNode->m_tRT.m_Point[0].x, pNode->m_tRT.m_Center.y };

    TNode* pNewNode = CreateNode(pNode,
        pNode->m_tRT.m_Point[0].x,
        pNode->m_tRT.m_Point[0].y,
        pNode->m_tRT.m_Half.x,
        pNode->m_tRT.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    pNewNode = CreateNode(pNode,
        vTC.x,
        vTC.y,
        pNode->m_tRT.m_Half.x,
        pNode->m_tRT.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    pNewNode = CreateNode(pNode,
        pNode->m_tRT.m_Center.x,
        pNode->m_tRT.m_Center.y,
        pNode->m_tRT.m_Half.x,
        pNode->m_tRT.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    pNewNode = CreateNode(pNode,
        vLC.x,
        vLC.y,
        pNode->m_tRT.m_Half.x,
        pNode->m_tRT.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);


    std::cout << "[" << pNode->m_iDepth << "]"
        << pNode->m_tRT.v.x << ","
        << pNode->m_tRT.v.y << ","
        << pNode->m_tRT.m_fWidth << ","
        << pNode->m_tRT.m_fHeight << std::endl;

    for (int i = 0; i < pNode->m_pChild.size(); i++)
    {
        BuildTree(pNode->m_pChild[i]);
    }
}
void TQuadtree::BuildQuadTree(TSpaceData data)
{
    m_pRootNode = CreateNode(nullptr, data);
    BuildTree(m_pRootNode);
}
