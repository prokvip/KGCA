#include "pch.h"
#include "TInterface.h"
#include "TInput.h"
void TInterface::FadeInOut(float fAlpha)
{
    m_VertexList[0].c = { 1,1,1,fAlpha };
    m_VertexList[1].c = { 1,1,1,fAlpha };
    m_VertexList[2].c = { 1,1,1,fAlpha };
    m_VertexList[3].c = { 1,1,1,fAlpha };
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
}
bool TInterface::Frame()
{
    POINT ptMouse = I_Input.m_ptPos;
    if (TCollision::RectToPoint(m_rtCollision, ptMouse))
    {
        m_CurrentState = UI_HOVER;
        m_pCurrentTex = m_EventState.m_pStateList[UI_HOVER];
        if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH ||
            I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
        {
            m_CurrentState = UI_PUSH;
            m_pCurrentTex = m_EventState.m_pStateList[UI_PUSH];
        }
        if (I_Input.GetKey(VK_LBUTTON) == KEY_UP)
        {
            m_CurrentState = UI_SELECT;
        }
    }
    else
    {
        m_pCurrentTex = m_EventState.m_pStateList[UI_NORMAL];
    }
    ScreenToNDC();
    UpdateVertexBuffer();
    return true;
}
void TInterface::AddChild(TInterface* pUI)
{
    m_pChildList.push_back(pUI);
};
bool  TInterface::SetTextueState(const std::vector<W_STR>& texStateList)
{
    if (texStateList.size() <= 0)
    {
        m_pCurrentTex = m_pTexture;
        m_EventState.m_pStateList[0] = m_pTexture;
        m_EventState.m_pStateList[1] = m_pTexture;
        m_EventState.m_pStateList[2] = m_pTexture;
        m_EventState.m_pStateList[3] = m_pTexture;
        return true;
    }

    TTexture* pTexture = nullptr;
    if (!texStateList[0].empty())
    {
        pTexture = I_Tex.Load(texStateList[0]);
        m_pCurrentTex           = pTexture;
        m_EventState.m_pStateList[UI_NORMAL] = pTexture;
    }
    if (!texStateList[1].empty())
    {
        pTexture = I_Tex.Load(texStateList[1]);
        m_EventState.m_pStateList[UI_HOVER] = pTexture;
    }
    if (!texStateList[2].empty())
    {
        pTexture = I_Tex.Load(texStateList[2]);
        m_EventState.m_pStateList[UI_PUSH] = pTexture;
    }
    if (!texStateList[3].empty())
    {
        pTexture = I_Tex.Load(texStateList[3]);
        m_EventState.m_pStateList[UI_DIS] = pTexture;
    }
    return true;
}
bool  TInterface::SetAttribute( TVector2 vPos, TRect rt, 
                                const std::vector<W_STR>& texStateList)
{
    Init();
    SetRect(rt);
    SetPosition(vPos);
    SetTextueState(texStateList);    
    return true;
}
bool  TInterface::SetAttribute(TVector2 vPos, TRect rt, TSprite* pSprite)
{
    Init();
    SetRect(rt);
    SetPosition(vPos);
    m_pSprite = pSprite;
    if (pSprite->m_texArray.size() > 0)
    {
        m_pCurrentTex = m_pTexture;
        m_EventState.m_pStateList = pSprite->m_pTexArray;
        return true;
    }    
    return true;
}
bool  TInterface::SetAttribute(TVector2 vPos, const std::vector<W_STR>& texStateList)
{
    Init();
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    TRect rt = { 0,0,(float)m_ptImageSize.x, (float)m_ptImageSize.y };
    SetRect(rt);
    SetPosition(vPos);
    SetTextueState(texStateList);
    return true;
}
bool  TInterface::SetDrawList(
    float fScaleX0, float fScaleX1,
    float fScaleY0, float fScaleY1,
    float fScaleU0, float fScaleU1,
    float fScaleV0, float fScaleV1)
{
    m_rtDrawList.resize(9);
    for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
    {
        m_rtDrawList[iSub] = new TInterface;
        m_rtDrawList[iSub]->Create(m_pd3dDevice, m_pImmediateContext,
            m_szShaderName,
            m_szTextureName);
    }

    TRect rt = m_rtCollision;
    TVector2  vCenter = m_vPos;
    float fX[4];
    fX[0] = m_rtCollision.x1;
    fX[1] = fX[0] + m_rtCollision.w * fScaleX0;
    fX[2] = m_rtCollision.x2 - (m_rtCollision.w * fScaleX1);
    fX[3] = m_rtCollision.x2;
    float fY[4];
    fY[0] = m_rtCollision.y1;
    fY[1] = fY[0] + m_rtCollision.h * fScaleY0;
    fY[2] = m_rtCollision.y2 - (m_rtCollision.h * fScaleY1);
    fY[3] = m_rtCollision.y2;
    float fU[4];
    fU[0] = 0.0f;
    fU[1] = fU[0] + fScaleU0;
    fU[2] = 1.0f - fU[1];
    fU[3] = 1.0f;
    float fV[4];
    fV[0] = 0.0f;
    fV[1] = fV[0] + fScaleV0;
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

bool  TButton::Init()
{
    return true;
}
bool  TButton::Frame()
{
    TInterface::Frame();
  /*  m_VertexList[0].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f+0.5f };
    m_VertexList[1].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[2].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[3].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[0].t = { 0.0f, 0.0f };
    m_VertexList[1].t = { 1.0f, 0.0f };
    m_VertexList[2].t = { 0.0f, 1.0f };
    m_VertexList[3].t = { 1.0f, 1.0f };
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
  */  return true;
}
bool  TButton::Render(ID3D11DeviceContext* pContext)
{
    TBaseObject::PreRender(pContext);
    pContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
    TBaseObject::PostRender(pContext);
    return true;
}
bool  TButton::Release()
{
    TInterface::Release();
    return true;
}
void  TButton::SetRect(TRect rt)
{
    m_rtInit = rt;
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    m_rtUV.x1 = 0.0f;
    m_rtUV.y1 = 0.0f;
    m_rtUV.w = 1.0f;
    m_rtUV.h = 1.0f;
}
bool  TListControl::Init()
{
    return true;
}
bool  TListControl::Frame()
{
    for (auto data : m_pChildList)
    {
       TVector2 pos = data->m_vPos + m_vOffsetPos;
       data->SetPosition(pos);
       data->Frame();
    }
    return true;
}
bool  TListControl::Render(ID3D11DeviceContext* pContext)
{
    TBaseObject::PreRender(pContext);
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
    TBaseObject::PostRender(pContext);

    for (auto data : m_pChildList)
    {
        data->Render(pContext);
    }
    return true;
}
bool  TListControl::Release()
{
    for (auto data : m_pChildList)
    {
        data->Release();
        delete data;
    }
    TInterface::Release();
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

bool  TDialog::Init()
{
    return true;
}
bool  TDialog::Render(ID3D11DeviceContext* pContext)
{
    for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
    {
        m_rtDrawList[iSub]->Render(pContext);
    }
    for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
    {
        m_pChildList[iChild]->Render(pContext);
    }
    return true;
}
bool  TDialog::Frame()
{
    TInterface::Frame();

    for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
    {
        TVector2 pos = m_rtDrawList[iSub]->m_vPos + m_vOffsetPos;
        m_rtDrawList[iSub]->SetPosition(pos);
    }
    for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
    {
        TVector2 pos = m_pChildList[iChild]->m_vPos + m_vOffsetPos;
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
    TInterface::Release();
    return true;
}

