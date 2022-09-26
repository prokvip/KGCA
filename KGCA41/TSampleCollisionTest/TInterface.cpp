#include "TInterface.h"
#include "TInput.h"
bool  TDialog::SetDrawList(
    float fScaleX0, float fScaleX1,
    float fScaleY0, float fScaleY1,
    float fScaleU0, float fScaleU1,
    float fScaleV0, float fScaleV1 )
{
    m_rtDrawList.resize(9);
    for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
    {
        m_rtDrawList[iSub] = new TInterface;
        m_rtDrawList[iSub]->Create(m_pd3dDevice, m_pImmediateContext,
            m_szShaderName,
            m_szTextureName);
    }

    std::vector<W_STR> zeroState;
    TRect rt = m_rtCollision;
    TVector2D  vCenter = m_vPos;
    float fX[4];
    fX[0] = m_rtCollision.x1;
    fX[1] = fX[0]+m_rtCollision.w* fScaleX0;
    fX[2] = m_rtCollision.x2 -(m_rtCollision.w * fScaleX1);
    fX[3] = m_rtCollision.x2;
    float fY[4];
    fY[0] = m_rtCollision.y1;
    fY[1] = fY[0]+m_rtCollision.h * fScaleY0;
    fY[2] = m_rtCollision.y2 - (m_rtCollision.h * fScaleY1);
    fY[3] = m_rtCollision.y2;
    float fU[4];
    fU[0] = 0.0f;
    fU[1] = fU[0]+fScaleU0;
    fU[2] = 1.0f - fU[1];
    fU[3] = 1.0f;
    float fV[4];
    fV[0] = 0.0f;
    fV[1] = fV[0]+fScaleV0;
    fV[2] = 1.0f - fV[1];
    fV[3] = 1.0f;

    vCenter.x = (fX[0] + fX[1]) * 0.5f;
    vCenter.y = (fY[0] + fY[1]) * 0.5f;
    m_rtDrawList[0]->m_rtInit.Set(0, 0, fX[1] - fX[0], fY[1] - fY[0]);    
    m_rtDrawList[0]->m_rtUV.x1 = fU[0];
    m_rtDrawList[0]->m_rtUV.y1 = fV[0];
    m_rtDrawList[0]->m_rtUV.w = fU[1] - fU[0];
    m_rtDrawList[0]->m_rtUV.h = fV[1] - fV[0];
    m_rtDrawList[0]->SetPosition(vCenter);

    vCenter.x = (fX[2] + fX[1]) * 0.5f;    
    m_rtDrawList[1]->m_rtInit.Set(fX[1], fY[0], fX[2] - fX[1], fY[1] - fY[0]);
    m_rtDrawList[1]->m_rtUV.x1 = fU[1];
    m_rtDrawList[1]->m_rtUV.y1 = fV[0];
    m_rtDrawList[1]->m_rtUV.w = fU[2] - fU[1];
    m_rtDrawList[1]->m_rtUV.h = fV[1] - fV[0];
    m_rtDrawList[1]->SetPosition(vCenter);

    vCenter.x = (fX[3] + fX[2]) * 0.5f;
    m_rtDrawList[2]->m_rtInit.Set(fX[2], fY[0], fX[3] - fX[2], fY[1] - fY[0]);
    m_rtDrawList[2]->m_rtUV.x1 = fU[2];
    m_rtDrawList[2]->m_rtUV.y1 = fV[0];
    m_rtDrawList[2]->m_rtUV.w = fU[3] - fU[2];
    m_rtDrawList[2]->m_rtUV.h = fV[1] - fV[0];
    m_rtDrawList[2]->SetPosition(vCenter);
   
    // 2
    vCenter.x = (fX[0] + fX[1]) * 0.5f;
    vCenter.y = (fY[2] + fY[1]) * 0.5f;
    m_rtDrawList[3]->m_rtInit.Set(0, 0, fX[1] - fX[0], fY[2] - fY[1]);
    m_rtDrawList[3]->m_rtUV.x1 = fU[0];
    m_rtDrawList[3]->m_rtUV.y1 = fV[1];
    m_rtDrawList[3]->m_rtUV.w = fU[1] - fU[0];
    m_rtDrawList[3]->m_rtUV.h = fV[2] - fV[1];
    m_rtDrawList[3]->SetPosition(vCenter);

    vCenter.x = (fX[2] + fX[1]) * 0.5f;
    m_rtDrawList[4]->m_rtInit.Set(fX[1], fY[1], fX[2] - fX[1], fY[2] - fY[1]);
    m_rtDrawList[4]->m_rtUV.x1 = fU[1];
    m_rtDrawList[4]->m_rtUV.y1 = fV[1];
    m_rtDrawList[4]->m_rtUV.w = fU[2] - fU[1];
    m_rtDrawList[4]->m_rtUV.h = fV[2] - fV[1];
    m_rtDrawList[4]->SetPosition(vCenter);

    vCenter.x = (fX[3] + fX[2]) * 0.5f;
    m_rtDrawList[5]->m_rtInit.Set(fX[2], fY[1], fX[3] - fX[2], fY[2] - fY[1]);
    m_rtDrawList[5]->m_rtUV.x1 = fU[2];
    m_rtDrawList[5]->m_rtUV.y1 = fV[1];
    m_rtDrawList[5]->m_rtUV.w = fU[3] - fU[2];
    m_rtDrawList[5]->m_rtUV.h = fV[2] - fV[1];
    m_rtDrawList[5]->SetPosition(vCenter);

    // 3
    vCenter.x = (fX[0] + fX[1]) * 0.5f;
    vCenter.y = (fY[3] + fY[2]) * 0.5f;
    m_rtDrawList[6]->m_rtInit.Set(0, 0, fX[1] - fX[0], fY[3] - fY[2]);
    m_rtDrawList[6]->m_rtUV.x1 = fU[0];
    m_rtDrawList[6]->m_rtUV.y1 = fV[2];
    m_rtDrawList[6]->m_rtUV.w = fU[1] - fU[0];
    m_rtDrawList[6]->m_rtUV.h = fV[3] - fV[2];
    m_rtDrawList[6]->SetPosition(vCenter);

    vCenter.x = (fX[2] + fX[1]) * 0.5f;
    m_rtDrawList[7]->m_rtInit.Set(fX[1], fY[2], fX[2] - fX[1], fY[3] - fY[2]);
    m_rtDrawList[7]->m_rtUV.x1 = fU[1];
    m_rtDrawList[7]->m_rtUV.y1 = fV[2];
    m_rtDrawList[7]->m_rtUV.w = fU[2] - fU[1];
    m_rtDrawList[7]->m_rtUV.h = fV[3] - fV[2];
    m_rtDrawList[7]->SetPosition(vCenter);

    vCenter.x = (fX[3] + fX[2]) * 0.5f;
    m_rtDrawList[8]->m_rtInit.Set(fX[2], fY[2], fX[3] - fX[2], fY[3] - fY[2]);
    m_rtDrawList[8]->m_rtUV.x1 = fU[2];
    m_rtDrawList[8]->m_rtUV.y1 = fV[2];
    m_rtDrawList[8]->m_rtUV.w = fU[3] - fU[2];
    m_rtDrawList[8]->m_rtUV.h = fV[3] - fV[2];
    m_rtDrawList[8]->SetPosition(vCenter);
    return true;
}
void  TInterface::Rotation()
{
    float fRadian = DegreeToRadian(m_fAngleDegree);
    for (int iV = 0; iV < 4; iV++)
    {
        m_InitVertexList[iV].p.x = m_VertexList[iV].p.x * cos(fRadian) - m_VertexList[iV].p.y * sin(fRadian);
        m_InitVertexList[iV].p.y = m_VertexList[iV].p.x * sin(fRadian) + m_VertexList[iV].p.y * cos(fRadian);
    }
    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL, NULL, &m_InitVertexList.at(0), 0, 0);
}
bool  TInterface::SetAttribute(TVector2D vPos, TRect rt, 
    std::vector<W_STR>& texStateList)
{
    Init();
    SetRect(rt);
    SetPosition(vPos);

    if (texStateList.size() <= 0)
    {
        m_pCurrentTex = m_pTexture;
        m_pStateList.push_back(m_pTexture);
        m_pStateList.push_back(m_pTexture);
        m_pStateList.push_back(m_pTexture);
        m_pStateList.push_back(m_pTexture);
        return true;
    }

    TTexture* pTexture = nullptr;
    if (!texStateList[0].empty() )
    {
        pTexture = I_Tex.Load(texStateList[0]);
        m_pCurrentTex = pTexture;
        m_pStateList.push_back(pTexture);
    }
    if (!texStateList[1].empty())
    {
        pTexture = I_Tex.Load(texStateList[1]);
        m_pStateList.push_back(pTexture);
    }
    if (!texStateList[2].empty())
    {
        pTexture = I_Tex.Load(texStateList[2]);
        m_pStateList.push_back(pTexture);
    }
    if (!texStateList[3].empty())
    {
        pTexture = I_Tex.Load(texStateList[3]);
        m_pStateList.push_back(pTexture);
    }
    return true;
}
void  TDialog::SetRect(TRect rt)
{
    m_rtInit = rt;
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    m_rtUV.x1 = rt.x1 / m_ptImageSize.x;
    m_rtUV.y1 = rt.y1 / m_ptImageSize.y;
    m_rtUV.w = rt.w / m_ptImageSize.x;
    m_rtUV.h = rt.h / m_ptImageSize.y;
}
bool  TDialog::Render()
{
    for (int iSub = 0; iSub < 9; iSub++)
    {
        m_rtDrawList[iSub]->Render();
    }
    for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
    {
        m_pChildList[iChild]->Render();
    }
    return true;
}
bool  TDialog::Frame()
{
    POINT ptMouse = I_Input.m_ptPos;
    if (TCollision::RectToPoint(m_rtCollision, ptMouse))
    {
        m_CurrentState = UI_HOVER;
        m_pCurrentTex = m_pStateList[UI_HOVER];
        if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH ||
            I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
        {
            m_CurrentState = UI_PUSH;
            m_pCurrentTex = m_pStateList[UI_PUSH];
        }
        if (I_Input.GetKey(VK_LBUTTON) == KEY_UP)
        {
            m_CurrentState = UI_SELECT;
        }
    }
    else
    {        
        m_pCurrentTex = m_pStateList[UI_NORMAL];
    }
    for (int iSub = 0; iSub < 9; iSub++)
    {
        TVector2D pos = m_rtDrawList[iSub]->m_vPos + m_vOffsetPos;        
        m_rtDrawList[iSub]->SetPosition(pos);
    }
    for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
    {
        TVector2D pos = m_pChildList[iChild]->m_vPos + m_vOffsetPos;
        m_pChildList[iChild]->SetPosition(pos);
        m_pChildList[iChild]->Frame();
    }
    return true;
}
bool  TDialog::Release()
{
    for (auto data : m_rtDrawList)
    {
        data->Release();
        delete data;
    }
    for (auto data : m_pChildList)
    {
        data->Release();
        delete data;
    }
    TObject2D::Release();
    return true;
}
bool  TListControl::Init()
{
    return true;
}

bool  TButton::Init()
{
    return true;
}
bool  TButton::Frame()
{
    POINT ptMouse = I_Input.m_ptPos;
    if (TCollision::RectToPoint( m_rtCollision, ptMouse))
    {
        m_CurrentState = UI_HOVER;
        m_pCurrentTex = m_pStateList[UI_HOVER];
        if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH ||
            I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
        {
            m_CurrentState = UI_PUSH;
            m_pCurrentTex = m_pStateList[UI_PUSH];
        }
        if (I_Input.GetKey(VK_LBUTTON) == KEY_UP)
        {
            m_CurrentState = UI_SELECT;
        }
    }
    else
    {
        m_pCurrentTex = m_pStateList[UI_NORMAL];
    }

    /*m_VertexList[0].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f+0.5f };
    m_VertexList[1].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[2].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[3].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };*/
    m_VertexList[0].t = { 0.0f, 0.0f };
    m_VertexList[1].t = { 1.0f, 0.0f };
    m_VertexList[2].t = { 0.0f, 1.0f };
    m_VertexList[3].t = { 1.0f, 1.0f };
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
    return true;
}
bool TButton::Render()
{
    TBaseObject::PreRender();
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
    TBaseObject::PostRender();
    return true;
}
bool TButton::Release()
{
    TObject2D::Release();
    return true;
}

bool  TListControl::Frame()
{
    for (auto data : m_pChildList)
    {
       TVector2D pos = data->m_vPos + m_vOffsetPos;
       data->SetPosition(pos);
       data->Frame();
    }
    return true;
}
bool TListControl::Render()
{
    TBaseObject::PreRender();
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
    TBaseObject::PostRender();

    for (auto data : m_pChildList)
    {
        data->Render();
    }
    return true;
}
bool TListControl::Release()
{
    for (auto data : m_pChildList)
    {
        data->Release();
        delete data;
    }
    TObject2D::Release();
    return true;
}
void  TListControl::SetRect(TRect rt)
{
    m_rtInit = rt;
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    m_rtUV.x1 = 0.0f;
    m_rtUV.y1 = 0.0f;
    m_rtUV.w = 1.0f;
    m_rtUV.h = 1.0f;
}
