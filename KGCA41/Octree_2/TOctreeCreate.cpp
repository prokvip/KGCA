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
    // 0, 0, 0  600, 600 , 600
    //  front z = 0
    // v0-------vTC1-------v1
    // |                  |
    // |                  |
    // vLC1      vC       vRC1
    // |                  |
    // |                  |
    // v3       vBC1       v2
    // 
    // middle z= 300
    // C_v0-------vTC2-------C_v1
    // |                  |
    // |                  |
    // C_vLC2      vC       vRC2
    // |                  |
    // |                  |
    // C_v3       vBC2       C_v2
    
    // back z= 600
    // v4-------vTC3-------v5
    // |                  |
    // |                  |
    // vLC3      vC       vRC3
    // |                  |
    // |                  |
    // v7       vBC3      v6

    TVector3 vTC1 = { pNode->m_tBox.m_Center.x, pNode->m_tBox.m_Point[0].y, 
                     pNode->m_tBox.m_Min.z };
    TVector3 vRC1 = { pNode->m_tBox.m_Point[1].x, pNode->m_tBox.m_Center.y,
                    pNode->m_tBox.m_Min.z };
    TVector3 vBC1 = { pNode->m_tBox.m_Center.x, pNode->m_tBox.m_Point[3].y,
                    pNode->m_tBox.m_Min.z };
    TVector3 vLC1 = { pNode->m_tBox.m_Point[0].x, pNode->m_tBox.m_Center.y,
                    pNode->m_tBox.m_Min.z };

    TVector3 vTC2 = { pNode->m_tBox.m_Center.x, pNode->m_tBox.m_Point[0].y,
                    pNode->m_tBox.m_Center.z };
    TVector3 vRC2 = { pNode->m_tBox.m_Point[1].x, pNode->m_tBox.m_Center.y,
                    pNode->m_tBox.m_Center.z };
    TVector3 vBC2 = { pNode->m_tBox.m_Center.x, pNode->m_tBox.m_Point[3].y,
                    pNode->m_tBox.m_Center.z };
    TVector3 vLC2 = { pNode->m_tBox.m_Point[0].x, pNode->m_tBox.m_Center.y,
                    pNode->m_tBox.m_Center.z };

    TVector3 vTC3 = { pNode->m_tBox.m_Center.x, pNode->m_tBox.m_Point[0].y,
                    pNode->m_tBox.m_Max.z };
    TVector3 vRC3 = { pNode->m_tBox.m_Point[1].x, pNode->m_tBox.m_Center.y,
                    pNode->m_tBox.m_Max.z };
    TVector3 vBC3 = { pNode->m_tBox.m_Center.x, pNode->m_tBox.m_Point[3].y,
                    pNode->m_tBox.m_Max.z };
    TVector3 vLC3 = { pNode->m_tBox.m_Point[0].x, pNode->m_tBox.m_Center.y,
                    pNode->m_tBox.m_Max.z };
    
    

    // 0
    TNode* pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Point[0].x, pNode->m_tBox.m_Point[0].y,  pNode->m_tBox.m_Point[0].z,
        pNode->m_tBox.m_Half.x,   pNode->m_tBox.m_Half.y,   pNode->m_tBox.m_Half.z);

    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    // 1
    pNewNode = CreateNode(pNode,
        vTC1.x,vTC1.y, vTC1.z,
        pNode->m_tBox.m_Half.x,  
        pNode->m_tBox.m_Half.y,   
        pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //2
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x,
        pNode->m_tBox.m_Center.y,
        pNode->m_tBox.m_Min.z,
        pNode->m_tBox.m_Half.x,
        pNode->m_tBox.m_Half.y,
        pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //3
    pNewNode = CreateNode(pNode,
        vLC1.x, vLC1.y,vLC1.z,
        pNode->m_tBox.m_Half.x,  pNode->m_tBox.m_Half.y,   pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    // 4
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Point[0].x, 
        pNode->m_tBox.m_Point[0].y, 
        pNode->m_tBox.m_Center.z,
        pNode->m_tBox.m_Half.x, pNode->m_tBox.m_Half.y, pNode->m_tBox.m_Half.z);

    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    // 5
    pNewNode = CreateNode(pNode,
        vTC2.x, vTC2.y, vTC2.z,
        pNode->m_tBox.m_Half.x, pNode->m_tBox.m_Half.y, pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //6
    pNewNode = CreateNode(pNode,
        pNode->m_tBox.m_Center.x,
        pNode->m_tBox.m_Center.y,
        pNode->m_tBox.m_Center.z,
        pNode->m_tBox.m_Half.x,
        pNode->m_tBox.m_Half.y,
        pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);
    //7
    pNewNode = CreateNode(pNode,
        vLC2.x, vLC2.y, vLC2.z,
        pNode->m_tBox.m_Half.x, pNode->m_tBox.m_Half.y, pNode->m_tBox.m_Half.z);
    pNode->m_pChild.push_back(pNewNode);
    m_NodeArrayList.push_back(pNode);

    std::wcout << L"[" << pNode->m_iDepth << L"]"
        << pNode->m_tBox.x << L","
        << pNode->m_tBox.y << L","
        << pNode->m_tBox.z << L","
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
