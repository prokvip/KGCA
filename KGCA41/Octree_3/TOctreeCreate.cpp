#include "TOctree.h"
TNode* TOctree::CreateNode(TNode* pParent, 
    float x, float y, float z,
    float w, float h, float d)
{
    TNode* pNode = new TNode(m_iNumNodeCounter++);
    pNode->SetParent(pParent);
    pNode->m_tBox.Set(x,y,z, w, h, d);
    return pNode;
}
TNode* TOctree::CreateNode(TNode* pParent, TSpaceData data)
{
    TNode* pNode = new TNode(m_iNumNodeCounter++);
    pNode->SetParent(pParent);
    pNode->m_tBox.Set(data.p3, data.w, data.h, data.z);
    return pNode;
}
void TOctree::BuildTree(TNode* pNode)
{
    if (pNode->m_iDepth > 1)
    {
        return;
    }
    // 0
    TVector3 vHalf = pNode->m_tBox.m_Half;
    vHalf = vHalf * 0.5f;    
    TNode* pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x - vHalf.x ,
        pNode->m_tBox.m_Center.y + vHalf.y,
        pNode->m_tBox.m_Center.z - vHalf.z,
        pNode->m_tBox.m_Half.x,   
        pNode->m_tBox.m_Half.y,   
        pNode->m_tBox.m_Half.z);

    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    // 1
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x + vHalf.x,
        pNode->m_tBox.m_Center.y + vHalf.y,
        pNode->m_tBox.m_Center.z - vHalf.z,
        pNode->m_tBox.m_Half.x,  
        pNode->m_tBox.m_Half.y,   
        pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //2
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x + vHalf.x,
        pNode->m_tBox.m_Center.y - vHalf.y,
        pNode->m_tBox.m_Center.z - vHalf.z,
        pNode->m_tBox.m_Half.x,
        pNode->m_tBox.m_Half.y,
        pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //3
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x - vHalf.x,
        pNode->m_tBox.m_Center.y - vHalf.y,
        pNode->m_tBox.m_Center.z - vHalf.z,
        pNode->m_tBox.m_Half.x,  pNode->m_tBox.m_Half.y,   pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    // 4
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x - vHalf.x,
        pNode->m_tBox.m_Center.y + vHalf.y,
        pNode->m_tBox.m_Center.z + vHalf.z,
        pNode->m_tBox.m_Half.x, pNode->m_tBox.m_Half.y, pNode->m_tBox.m_Half.z);

    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    // 5
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x + vHalf.x,
        pNode->m_tBox.m_Center.y + vHalf.y,
        pNode->m_tBox.m_Center.z + vHalf.z,
        pNode->m_tBox.m_Half.x, pNode->m_tBox.m_Half.y, pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //6
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x + vHalf.x,
        pNode->m_tBox.m_Center.y - vHalf.y,
        pNode->m_tBox.m_Center.z + vHalf.z,
        pNode->m_tBox.m_Half.x,
        pNode->m_tBox.m_Half.y,
        pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //7
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x - vHalf.x,
        pNode->m_tBox.m_Center.y - vHalf.y,
        pNode->m_tBox.m_Center.z + vHalf.z,
        pNode->m_tBox.m_Half.x, pNode->m_tBox.m_Half.y, pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    std::wcout << L"[" << pNode->m_iDepth << L"]"
        << pNode->m_tBox.v.x << L","
        << pNode->m_tBox.v.y << L","
        << pNode->m_tBox.v.z << L","
        << pNode->m_tBox.m_fWidth << L","
        << pNode->m_tBox.m_fHeight<< L","
        << pNode->m_tBox.m_fDepth << std::endl;

    for (int i = 0; i < pNode->m_pChild.size(); i++)
    {
        BuildTree(pNode->m_pChild[i]);
    }
}
void TOctree::BuildOctTree(
    TSpaceData data)
{
    m_pRootNode = CreateNode(nullptr, data);
    BuildTree(m_pRootNode);
}
