#include "pch.h"
#include "TQuadtree.h"
#include "TCollision.h"

void TQuadtree::Splatting(TVector3 vIntersection, UINT iSplattingTexIndex, float fSplattingRadius)
{
    UINT m_iSplattingIndex = iSplattingTexIndex;
    UINT const DataSize = sizeof(BYTE) * 4;
    UINT const RowPitch = DataSize * 1024;
    UINT const DepthPitch = 0;

    // pick data    ->  texture data
    // 0 ~ 64      ->   0 ~ 1
    // - 32 ~ +32  ->   0 ~ 1024 -> 0 ~ 1
    TVector2 vTexIndex;
    TVector2 vUV;
    TVector2 vMaxSize = { +32, +32 };
    UINT     iTexSize = 1024;
    TVector3 vTexPos;
    TVector3 vPickPos = vIntersection;

    m_iSplattingIndex = rand() % 4;

    for (UINT y = 0; y < iTexSize; y++)
    {
        vTexIndex.y = y;
        for (UINT x = 0; x < iTexSize; x++)
        {
            vTexIndex.x = x;
            // -1 ~ +1
            vUV = TVector2((vTexIndex.x / (float)iTexSize) * 2.0f - 1.0f,
                -(vTexIndex.y / (float)iTexSize * 2.0f - 1.0f));
            // -32 ~ +32
            vTexPos = TVector3(vUV.x * vMaxSize.x, 0.0f, vUV.y * vMaxSize.y);
            BYTE* pixel = &m_fAlphaData[iTexSize * y * 4 + x * 4];

            float fRadius = D3DXVec3Length(&(vPickPos - vTexPos));
            
            if (fRadius < fSplattingRadius)
            {
                float fDot = 1.0f - (fRadius / fSplattingRadius);
                if (m_iSplattingIndex == 0 && (fDot*255) > pixel[0])
                    pixel[0] = fDot * 255;// (cosf(g_fGameTimer) * 0.5f + 0.5f) * 255.0f;
                if (m_iSplattingIndex == 1 && (fDot * 255) > pixel[1])
                    pixel[1] = fDot * 255;//g
                if (m_iSplattingIndex == 2 && (fDot * 255) > pixel[2])
                    pixel[2] = fDot * 255;//b
                if (m_iSplattingIndex == 3 && (fDot * 255) > pixel[3])
                    pixel[3] = fDot * 255;//a
            }
        }
    }
    m_pMap->m_pImmediateContext->UpdateSubresource(m_pMaskAlphaTex.Get(), 0, nullptr, m_fAlphaData, RowPitch, DepthPitch);

}
HRESULT TQuadtree::CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight)
{
    HRESULT hr;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = dwWidth;
    td.Height = dwHeight;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;

    m_fAlphaData = new BYTE[dwWidth* dwHeight*4];
    for (UINT y = 0; y < dwHeight; y++)
    {
        for (UINT x = 0; x < dwWidth; x++)
        {
            BYTE* pixel = &m_fAlphaData[dwWidth*y*4 + x*4];
            pixel[0] = 0;//r
            pixel[1] = 0;//g
            pixel[2] = 0;//b
            pixel[3] = 0;//a
        }
    }
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = m_fAlphaData;
    initData.SysMemPitch = sizeof(BYTE) * 4 * dwWidth;
    initData.SysMemSlicePitch = 0;
    if (FAILED(hr = pDevice->CreateTexture2D(&td, &initData, m_pMaskAlphaTex.GetAddressOf())))
    {
        return hr;
    }
    if (FAILED(hr = pDevice->CreateShaderResourceView(m_pMaskAlphaTex.Get(), NULL, m_pMaskAlphaTexSRV.GetAddressOf())))
    {
        return hr;
    }
    
    return hr;
}
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

    CreateAlphaTexture(this->m_pMap->m_pd3dDevice, 1024, 1024);
    return true;
}
TNode* TQuadtree::FindNode(TNode* pNode, T_BOX tBox)
{
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            if (pNode->m_pChild[i]->m_tBox.vMin.x <= tBox.vMin.x &&
                pNode->m_pChild[i]->m_tBox.vMin.z <= tBox.vMin.z)
            {
                if (pNode->m_pChild[i]->m_tBox.vMax.x >= tBox.vMax.x &&
                    pNode->m_pChild[i]->m_tBox.vMax.z >= tBox.vMax.z)
                {
                    pNode = FindNode(pNode->m_pChild[i], tBox);
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
bool TQuadtree::AddObject(TWorldObject* pObj)
{
    m_ObjectList.push_back(pObj);

    TNode* pFindNode = FindNode(m_pRootNode, pObj->m_tBox);
    if (pFindNode != nullptr)
    {
        pFindNode->m_pDynamicObjectlist.push_back(pObj);
        return true;
    }
    return false;
}
UINT TQuadtree::SelectVertexList(T_BOX& tBox, std::vector<TNode*>& selectNodeList)
{
    for (auto node : m_pDrawLeafNodeList)
    {
        if (node != nullptr)
        {
            TCollisionType ret = TCollision::BoxToBox(node->m_tBox, tBox);
            if (ret > 0)
            {
                selectNodeList.push_back(node);
            }
        }
    }
    return selectNodeList.size();
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
void TQuadtree::VisibleNode(TNode* pNode)
{
    T_POSITION dwRet = m_pCamera->m_vFrustum.ClassifyTBox(pNode->m_tBox);
    if (P_FRONT== dwRet)// 완전포함.
    {
        m_pDrawLeafNodeList.push_back(pNode);
        return;
    }
    if(P_SPANNING== dwRet) // 걸쳐있다.
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
void TQuadtree::RenderObject(ID3D11DeviceContext* pContext, TNode* pNode)
{
    if (pNode==nullptr || pNode->m_bLeaf) return;
    pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    for (auto fbx : pNode->m_pDynamicObjectlist)
    {
        T_POSITION dwRet = m_pCamera->m_vFrustum.ClassifyTBox(fbx->m_tBox);
        if (P_FRONT > 0)// 완전포함.
        {
            fbx->SetMatrix(nullptr, &m_pMap->m_matView, &m_pMap->m_matProj);
            fbx->Render(pContext);
        }
    }
    for (int iChild = 0; iChild < 4; iChild++)
    {
        RenderObject(pContext,pNode->m_pChild[iChild]);
    }
}
void TQuadtree::RenderShadowObject(ID3D11DeviceContext* pContext, TNode* pNode)
{
    if (pNode == nullptr || pNode->m_bLeaf) return;
    for (auto fbx : pNode->m_pDynamicObjectlist)
    {
        T_POSITION dwRet = m_pCamera->m_vFrustum.ClassifyTBox(fbx->m_tBox);
        if (P_FRONT > 0)// 완전포함.
        {
            fbx->SetMatrix(nullptr, &m_pMap->m_matView, &m_pMap->m_matProj);
            fbx->RenderShadow(pContext);
        }
    }
    for (int iChild = 0; iChild < 4; iChild++)
    {
        RenderShadowObject(pContext, pNode->m_pChild[iChild]);
    }
}
bool TQuadtree::Render(ID3D11DeviceContext* pContext)
{
    for (auto node : m_pDrawLeafNodeList)
    {
        TVector3 vCam = m_pCamera->m_vPos;
        TVector3 vNode = node->m_tBox.vCenter;
        float fDistance = D3DXVec3Length(&(vCam - vNode));
        int iAmount = min(7, max(1,7-fabs(fDistance) / 10.0f));
        m_cbShadow.g_vTessellation = TVector4(iAmount, 0, 0, 0);
        pContext->UpdateSubresource(m_pShadowConstantBuffer.Get(), 0, NULL, &m_cbShadow, 0, 0);
        pContext->HSSetConstantBuffers(3, 1, m_pShadowConstantBuffer.GetAddressOf());

        m_pMap->PreRender(pContext);
        ID3D11ShaderResourceView* pSRV = nullptr;
        pContext->PSSetShaderResources(1, 1, m_pMaskAlphaTexSRV.GetAddressOf());
        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

        if (m_TexArray[0] != nullptr)
        {
            pContext->PSSetShaderResources(2, 1, &m_TexArray[1]->m_pTextureSRV);
            pContext->PSSetShaderResources(3, 1, &m_TexArray[2]->m_pTextureSRV);
            pContext->PSSetShaderResources(4, 1, &m_TexArray[3]->m_pTextureSRV);
            pContext->PSSetShaderResources(5, 1, &m_TexArray[4]->m_pTextureSRV);
        }
        pContext->IASetIndexBuffer(  node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        pContext->DrawIndexed(node->m_dwFace * 3, 0, 0);

        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        for (auto fbx : node->m_pDynamicObjectlist)
        {
            fbx->SetMatrix(nullptr, &m_pMap->m_matView, &m_pMap->m_matProj);
            fbx->Render(pContext);
        }
    }

    RenderObject(pContext,m_pRootNode);
    return true;
}
bool TQuadtree::RenderShadow(ID3D11DeviceContext* pContext, TCamera* pShadowCamera)
{
    TCamera* pSaveCamera = m_pCamera;
    m_pCamera = pShadowCamera;
    Frame();

    for (auto node : m_pDrawLeafNodeList)
    {
        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
        m_pMap->PreRender(pContext);

        TVector3 vCam = m_pCamera->m_vPos;
        TVector3 vNode = node->m_tBox.vCenter;
        float fDistance = D3DXVec3Length(&(vCam - vNode));
        int iAmount = min(7, max(1, 7 - fabs(fDistance) / 10.0f));
        m_cbShadow.g_vTessellation = TVector4(iAmount, 0, 0, 0);
        pContext->UpdateSubresource(m_pShadowConstantBuffer.Get(), 0, NULL, &m_cbShadow, 0, 0);
        pContext->HSSetConstantBuffers(3, 1, m_pShadowConstantBuffer.GetAddressOf());

        m_pMap->m_pImmediateContext->PSSetShader(NULL, NULL, 0);
        m_pMap->m_pImmediateContext->IASetIndexBuffer(node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pMap->m_pImmediateContext->DrawIndexed(node->m_dwFace * 3, 0, 0);

        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        for (auto fbx : node->m_pDynamicObjectlist)
        {
            fbx->SetMatrix(nullptr, &m_pMap->m_matView, &m_pMap->m_matProj);
            fbx->PreRender(pContext);
            pContext->PSSetShader(NULL, NULL, 0);
            fbx->PostRender(pContext);
        }
    }

    RenderShadowObject( pContext, m_pRootNode);

    m_pCamera = pSaveCamera;
    return true;
}
bool TQuadtree::Release()
{
    delete[] m_fAlphaData;
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