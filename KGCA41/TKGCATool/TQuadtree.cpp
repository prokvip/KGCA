#include "pch.h"
#include "TQuadtree.h"
#include "TCollision.h"
bool TQuadtree::Create(TCamera* pMainCamera, TMap* pMap, int iMaxDepth)
{
    m_pCamera = pMainCamera;
    m_pMap = pMap;
    m_iMaxDepth = iMaxDepth;
    m_pRootNode = new TNode(nullptr, m_pMap, 
        0, 
        pMap->m_iNumRows-1,
        pMap->m_iNumRows * (pMap->m_iNumCols - 1),
        pMap->m_iNumRows* pMap->m_iNumCols-1,
        pMap->m_iNumCols, pMap->m_iNumRows);
    BuildTree(m_pRootNode);
    return true;
}
TNode* TQuadtree::FindNode(TNode* pNode, TObject3D* pObj)
{
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            if (pNode->m_pChild[i]->m_tBox.vMin.x <= pObj->m_tBox.vMin.x &&
                pNode->m_pChild[i]->m_tBox.vMin.z <= pObj->m_tBox.vMin.z)
            {
                if (pNode->m_pChild[i]->m_tBox.vMax.x >= pObj->m_tBox.vMax.x &&
                    pNode->m_pChild[i]->m_tBox.vMax.z >= pObj->m_tBox.vMax.z)
                {
                    pNode = FindNode(pNode->m_pChild[i], pObj);
                    break;
                }
            }
            /*if (TCollision::BoxToInBox(   pNode->m_pChild[i]->m_tBox,
                                          pObj->m_tBox))
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
        //pNode->CreateIndexBuffer(   m_pMap->m_pd3dDevice,
        //                            m_pMap->m_pImmediateContext,
        //                            m_pMap->m_iNumRows,
        //                            m_pMap->m_iNumCols);
        pNode->m_bLeaf = true;
        m_pLeafNodeList.push_back(pNode);
        return;
    }
    pNode->CreateChildNode(pNode, m_pMap,
        m_pMap->m_iNumRows,
        m_pMap->m_iNumCols);

    BuildTree(pNode->m_pChild[0]);
    BuildTree(pNode->m_pChild[1]);
    BuildTree(pNode->m_pChild[2]);
    BuildTree(pNode->m_pChild[3]);
}
bool TQuadtree::IsSubDivide(TNode* pNode)
{
    if ((pNode->m_Corner[1] - pNode->m_Corner[0] )==1) return false;
    if (pNode->m_iDepth < m_iMaxDepth) return true;
    return false;
}
TNode* TQuadtree::VisibleNode(TNode* pNode)
{
    T_POSITION dwRet = m_pCamera->m_vFrustum.ClassifyTBox(pNode->m_tBox);
    if (P_FRONT== dwRet)// ��������.
    {
        m_pDrawLeafNodeList.push_back(pNode);
        return pNode;
    }
    if(P_SPANNING== dwRet) // �����ִ�.
    {
        if (pNode->m_bLeaf)
        {
            m_pDrawLeafNodeList.push_back(pNode);
        }
        else
        {
            for (int iNode = 0; iNode < 4; iNode++)
            {
                VisibleNode(pNode->m_pChild[iNode]);
            }
        }
   }   
}
bool TQuadtree::Frame()
{
    m_pDrawLeafNodeList.clear();
    //VisibleNode(m_pRootNode);
   
    for (auto node : m_pLeafNodeList)
    {
        if (m_pCamera->m_vFrustum.ClassifyTBox(node->m_tBox))
        {
            m_pDrawLeafNodeList.push_back(node);
        }
    }
    return true;
}
void TQuadtree::RenderObject(TNode* pNode)
{
    if (pNode==nullptr || pNode->m_bLeaf) return;
    for (auto fbx : pNode->m_pDynamicObjectlist)
    {
        T_POSITION dwRet = m_pCamera->m_vFrustum.ClassifyTBox(fbx->m_tBox);
        if (P_FRONT > 0)// ��������.
        {
            fbx->SetMatrix(nullptr, &m_pMap->m_matView, &m_pMap->m_matProj);
            fbx->Render();
        }
    }
    for (int iChild = 0; iChild < 4; iChild++)
    {
        RenderObject(pNode->m_pChild[iChild]);
    }
}
bool TQuadtree::Render()
{
    for (auto node : m_pDrawLeafNodeList)
    {
        m_pMap->PreRender();
        m_pMap->m_pImmediateContext->IASetIndexBuffer(
            node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pMap->m_pImmediateContext->DrawIndexed(node->m_dwFace * 3, 0, 0);

        for (auto fbx : node->m_pDynamicObjectlist)
        {
            fbx->SetMatrix(nullptr, &m_pMap->m_matView, &m_pMap->m_matProj);
            fbx->Render();
        }
    }

    RenderObject(m_pRootNode);
    return true;
}
bool TQuadtree::Release()
{
    if (m_pRootNode)
    {
        delete m_pRootNode;
        m_pRootNode = nullptr;
    }
    return true;
}
TQuadtree::~TQuadtree()
{
    if (m_pRootNode)
    {
        delete m_pRootNode;
        m_pRootNode = nullptr;
    }

}