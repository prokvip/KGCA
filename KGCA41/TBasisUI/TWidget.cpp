#include "TWidget.h"
namespace TUI
{
    TWidget* TWidget::m_pSelectEditWidget = nullptr;
    int      TWidget::g_iNumCounter = 0;
    //--------------------------------------------------------------------------------------
    bool  TWidget::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {        
        switch (uMsg)
        {
            // Mouse messages
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK:
        case WM_MOUSEWHEEL:
        {
            POINT mousePoint =
            {
                short(LOWORD(lParam)), short(HIWORD(lParam))
            };
            if (!ContainsPoint(mousePoint))
            {
                OnFocusOut();
                return false;
            }
            OnFocusIn();
        }
        }

        if (m_bHasFocus)
        {
            for (auto child : m_pChildList)
            {
                child->MsgProc(uMsg, wParam, lParam);
            }
        }
        return false;
    }
    bool  TWidget::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        bool bRet = false;
        for (auto child : m_pChildList)
        {
            bRet = child->HandleKeyboard(uMsg, wParam, lParam);           
        }
        return bRet;
    }
    bool  TWidget::HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
    {
        bool bRet = false;
        for (auto child : m_pChildList)
        {
            child->HandleMouse(uMsg, pt, wParam, lParam);           
        }
        return bRet;
    }
    bool  TWidget::Frame()
    {
        POINT ptMouse = I_InputUI.m_ptPos;        
        if (TCollision::RectToPoint(m_rtCollision, ptMouse))
        {                                   
            m_CurrentState = T_STATE_MOUSEOVER;
            m_pCurrentTex = m_pStateList[T_STATE_MOUSEOVER];
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH)
            {
                m_CurrentState = T_STATE_PRESSED;
                m_pCurrentTex = m_pStateList[T_STATE_PRESSED];
            }
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_HOLD)
            {
                m_CurrentState = T_STATE_HOLD;
                m_pCurrentTex = m_pStateList[T_STATE_PRESSED];
            }
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_UP)
            {
                m_CurrentState = T_STATE_SELECT;
                m_bToggle = !m_bToggle;
            }
        }
        else
        {
            m_CurrentState = T_STATE_NORMAL;
            m_pCurrentTex = m_pStateList[T_STATE_NORMAL];
        }
        return true;
    }
    void  TWidget::FadeInOut(float fAlpha)
    {
        m_VertexList[0].c = { 1,1,1,fAlpha };
        m_VertexList[1].c = { 1,1,1,fAlpha };
        m_VertexList[2].c = { 1,1,1,fAlpha };
        m_VertexList[3].c = { 1,1,1,fAlpha };
        m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
    }
    bool  TWidget::SetTextueState(const std::vector<W_STR>& texStateList)
    {
        if (texStateList.size() <= 0)
        {
            m_pCurrentTex = m_pTexture;
            m_pStateList[T_STATE_NORMAL] = m_pTexture;
            m_pStateList[T_STATE_MOUSEOVER] = m_pTexture;
            m_pStateList[T_STATE_PRESSED] = m_pTexture;
            m_pStateList[T_STATE_DISABLED] = m_pTexture;
            return true;
        }

        TTexture* pTexture = nullptr;
        if (texStateList.size() >= T_STATE_NORMAL && !texStateList[T_STATE_NORMAL].empty())
        {
            pTexture = I_TexUI.Load(texStateList[T_STATE_NORMAL]);
            m_pCurrentTex = pTexture;
            m_pStateList[T_STATE_NORMAL] = pTexture;
        }
        if (texStateList.size() >= T_STATE_MOUSEOVER && !texStateList[T_STATE_MOUSEOVER].empty())
        {
            pTexture = I_TexUI.Load(texStateList[T_STATE_MOUSEOVER]);
            m_pStateList[T_STATE_MOUSEOVER] = pTexture;
        }
        if (texStateList.size() >= T_STATE_PRESSED && !texStateList[T_STATE_PRESSED].empty())
        {
            pTexture = I_TexUI.Load(texStateList[T_STATE_PRESSED]);
            m_pStateList[T_STATE_PRESSED] = pTexture;
        }
        if (texStateList.size() >= T_STATE_DISABLED && !texStateList[T_STATE_DISABLED].empty())
        {
            pTexture = I_TexUI.Load(texStateList[T_STATE_DISABLED]);
            m_pStateList[T_STATE_DISABLED] = pTexture;
        }
        return true;
    }
    bool  TWidget::SetAttribute(TRect rt, const std::vector<W_STR>& texStateList)
    {
        SetRect(rt);
        SetPosition({ rt.cx,rt.cy });
        SetTextueState(texStateList);
        UpdateRects();
        return true;
    }
    bool  TWidget::SetAttribute(TVector2D vPos, const std::vector<W_STR>& texStateList)
    {
        TRect rt = { 0,0,0,0 };
        if (m_pTexture != nullptr)
        {
            m_ptImageSize.x = m_pTexture->m_Desc.Width;
            m_ptImageSize.y = m_pTexture->m_Desc.Height;
            rt = { 0,0,(float)m_ptImageSize.x, (float)m_ptImageSize.y };
        }
        SetRect(rt);
        SetPosition(vPos);
        SetTextueState(texStateList);
        UpdateRects();
        return true;
    }
    void  TWidget::Move(TVector2D vOffsetPos)
    {        
        TRect tRt = m_rtInitPos;
        //tRt += vOffsetPos;
        SetAttribute({vOffsetPos.x, vOffsetPos.y, tRt.w, tRt.h});
        //Move(vOffsetPos);
        for (auto data : m_pChildList)
        {
            data->Move(vOffsetPos);
        }
    }
    void  TWidget::Move(TRect tRt)
    {
        SetAttribute(tRt);
        for (auto data : m_pChildList)
        {
            data->Move(tRt);
        }
    }
    bool  TWidget::SetDrawList(
        float fScaleX0, float fScaleX1,
        float fScaleY0, float fScaleY1,
        float fScaleU0, float fScaleU1,
        float fScaleV0, float fScaleV1)
    {
        if (m_rtDrawList.size() == 0)
        {
            m_rtDrawList.resize(9);
            for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
            {
                m_rtDrawList[iSub] = new TWidget;
                m_rtDrawList[iSub]->Create(m_pd3dDevice, m_pImmediateContext, m_szShaderName, m_szTextureName);
            }
        }

        TRect rt = m_rtCollision;
        TVector2D  vCenter = m_vPos;
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

        TVector4D vOffsetUV = GetSpriteOffsetUV();
        // V0 + t * (V1 - V0)
        fU[0] = lerp(fU[0], vOffsetUV.x, vOffsetUV.z);
        fU[1] = lerp(fU[1], vOffsetUV.x, vOffsetUV.z);
        fU[2] = lerp(fU[2], vOffsetUV.x, vOffsetUV.z);
        fU[3] = lerp(fU[3], vOffsetUV.x, vOffsetUV.z);
        fV[0] = lerp(fV[0], vOffsetUV.y, vOffsetUV.w);
        fV[1] = lerp(fV[1], vOffsetUV.y, vOffsetUV.w);
        fV[2] = lerp(fV[2], vOffsetUV.y, vOffsetUV.w);
        fV[3] = lerp(fV[3], vOffsetUV.y, vOffsetUV.w);

        vCenter.x = (fX[0] + fX[1]) * 0.5f;
        vCenter.y = (fY[0] + fY[1]) * 0.5f;
        m_rtDrawList[0]->m_rtInitPos.Set(0, 0, fX[1] - fX[0], fY[1] - fY[0]);
        m_rtDrawList[0]->m_rtUV.x1 = fU[0];
        m_rtDrawList[0]->m_rtUV.y1 = fV[0];
        m_rtDrawList[0]->m_rtUV.w = fU[1] - fU[0];
        m_rtDrawList[0]->m_rtUV.h = fV[1] - fV[0];
        m_rtDrawList[0]->SetPosition(vCenter);

        vCenter.x = (fX[2] + fX[1]) * 0.5f;
        m_rtDrawList[1]->m_rtInitPos.Set(fX[1], fY[0], fX[2] - fX[1], fY[1] - fY[0]);
        m_rtDrawList[1]->m_rtUV.x1 = fU[1];
        m_rtDrawList[1]->m_rtUV.y1 = fV[0];
        m_rtDrawList[1]->m_rtUV.w = fU[2] - fU[1];
        m_rtDrawList[1]->m_rtUV.h = fV[1] - fV[0];
        m_rtDrawList[1]->SetPosition(vCenter);

        vCenter.x = (fX[3] + fX[2]) * 0.5f;
        m_rtDrawList[2]->m_rtInitPos.Set(fX[2], fY[0], fX[3] - fX[2], fY[1] - fY[0]);
        m_rtDrawList[2]->m_rtUV.x1 = fU[2];
        m_rtDrawList[2]->m_rtUV.y1 = fV[0];
        m_rtDrawList[2]->m_rtUV.w = fU[3] - fU[2];
        m_rtDrawList[2]->m_rtUV.h = fV[1] - fV[0];
        m_rtDrawList[2]->SetPosition(vCenter);

        // 2
        vCenter.x = (fX[0] + fX[1]) * 0.5f;
        vCenter.y = (fY[2] + fY[1]) * 0.5f;
        m_rtDrawList[3]->m_rtInitPos.Set(0, 0, fX[1] - fX[0], fY[2] - fY[1]);
        m_rtDrawList[3]->m_rtUV.x1 = fU[0];
        m_rtDrawList[3]->m_rtUV.y1 = fV[1];
        m_rtDrawList[3]->m_rtUV.w = fU[1] - fU[0];
        m_rtDrawList[3]->m_rtUV.h = fV[2] - fV[1];
        m_rtDrawList[3]->SetPosition(vCenter);

        vCenter.x = (fX[2] + fX[1]) * 0.5f;
        m_rtDrawList[4]->m_rtInitPos.Set(fX[1], fY[1], fX[2] - fX[1], fY[2] - fY[1]);
        m_rtDrawList[4]->m_rtUV.x1 = fU[1];
        m_rtDrawList[4]->m_rtUV.y1 = fV[1];
        m_rtDrawList[4]->m_rtUV.w = fU[2] - fU[1];
        m_rtDrawList[4]->m_rtUV.h = fV[2] - fV[1];
        m_rtDrawList[4]->SetPosition(vCenter);

        vCenter.x = (fX[3] + fX[2]) * 0.5f;
        m_rtDrawList[5]->m_rtInitPos.Set(fX[2], fY[1], fX[3] - fX[2], fY[2] - fY[1]);
        m_rtDrawList[5]->m_rtUV.x1 = fU[2];
        m_rtDrawList[5]->m_rtUV.y1 = fV[1];
        m_rtDrawList[5]->m_rtUV.w = fU[3] - fU[2];
        m_rtDrawList[5]->m_rtUV.h = fV[2] - fV[1];
        m_rtDrawList[5]->SetPosition(vCenter);

        // 3
        vCenter.x = (fX[0] + fX[1]) * 0.5f;
        vCenter.y = (fY[3] + fY[2]) * 0.5f;
        m_rtDrawList[6]->m_rtInitPos.Set(0, 0, fX[1] - fX[0], fY[3] - fY[2]);
        m_rtDrawList[6]->m_rtUV.x1 = fU[0];
        m_rtDrawList[6]->m_rtUV.y1 = fV[2];
        m_rtDrawList[6]->m_rtUV.w = fU[1] - fU[0];
        m_rtDrawList[6]->m_rtUV.h = fV[3] - fV[2];
        m_rtDrawList[6]->SetPosition(vCenter);

        vCenter.x = (fX[2] + fX[1]) * 0.5f;
        m_rtDrawList[7]->m_rtInitPos.Set(fX[1], fY[2], fX[2] - fX[1], fY[3] - fY[2]);
        m_rtDrawList[7]->m_rtUV.x1 = fU[1];
        m_rtDrawList[7]->m_rtUV.y1 = fV[2];
        m_rtDrawList[7]->m_rtUV.w = fU[2] - fU[1];
        m_rtDrawList[7]->m_rtUV.h = fV[3] - fV[2];
        m_rtDrawList[7]->SetPosition(vCenter);

        vCenter.x = (fX[3] + fX[2]) * 0.5f;
        m_rtDrawList[8]->m_rtInitPos.Set(fX[2], fY[2], fX[3] - fX[2], fY[3] - fY[2]);
        m_rtDrawList[8]->m_rtUV.x1 = fU[2];
        m_rtDrawList[8]->m_rtUV.y1 = fV[2];
        m_rtDrawList[8]->m_rtUV.w = fU[3] - fU[2];
        m_rtDrawList[8]->m_rtUV.h = fV[3] - fV[2];
        m_rtDrawList[8]->SetPosition(vCenter);
        return true;
    }
    bool  TWidget::SetDrawList(
        float fScaleX0, float fScaleX1,
        float fScaleY0, float fScaleY1,
        TSprite* pSprite)
    {
        SetSprite(pSprite);
        if (m_rtDrawList.size() == 0)
        {
            m_rtDrawList.resize(9);
            for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
            {
                m_rtDrawList[iSub] = new TWidget;
                m_rtDrawList[iSub]->Create(m_pd3dDevice, m_pImmediateContext,
                    pSprite->m_pShader, pSprite->m_pTexture);
            }
        }

        TVector2D  vCenter = m_vPos;
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

        vCenter.x = (fX[0] + fX[1]) * 0.5f;
        vCenter.y = (fY[0] + fY[1]) * 0.5f;
        m_rtDrawList[0]->m_rtInitPos.Set(0, 0, fX[1] - fX[0], fY[1] - fY[0]);

        TRect trt;
        RECT rt = pSprite->m_uvArray[0];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[0]->SetRectUV(trt);
        m_rtDrawList[0]->SetPosition(vCenter);

        vCenter.x = (fX[2] + fX[1]) * 0.5f;
        m_rtDrawList[1]->m_rtInitPos.Set(fX[1], fY[0], fX[2] - fX[1], fY[1] - fY[0]);
        rt = pSprite->m_uvArray[1];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[1]->SetRectUV(trt);
        m_rtDrawList[1]->SetPosition(vCenter);

        vCenter.x = (fX[3] + fX[2]) * 0.5f;
        m_rtDrawList[2]->m_rtInitPos.Set(fX[2], fY[0], fX[3] - fX[2], fY[1] - fY[0]);
        rt = pSprite->m_uvArray[2];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[2]->SetRectUV(trt);
        m_rtDrawList[2]->SetPosition(vCenter);

        // 2
        vCenter.x = (fX[0] + fX[1]) * 0.5f;
        vCenter.y = (fY[2] + fY[1]) * 0.5f;
        m_rtDrawList[3]->m_rtInitPos.Set(0, 0, fX[1] - fX[0], fY[2] - fY[1]);
        rt = pSprite->m_uvArray[3];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[3]->SetRectUV(trt);
        m_rtDrawList[3]->SetPosition(vCenter);

        vCenter.x = (fX[2] + fX[1]) * 0.5f;
        m_rtDrawList[4]->m_rtInitPos.Set(fX[1], fY[1], fX[2] - fX[1], fY[2] - fY[1]);
        rt = pSprite->m_uvArray[4];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[4]->SetRectUV(trt);
        m_rtDrawList[4]->SetPosition(vCenter);

        vCenter.x = (fX[3] + fX[2]) * 0.5f;
        m_rtDrawList[5]->m_rtInitPos.Set(fX[2], fY[1], fX[3] - fX[2], fY[2] - fY[1]);
        rt = pSprite->m_uvArray[5];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[5]->SetRectUV(trt);
        m_rtDrawList[5]->SetPosition(vCenter);

        // 3
        vCenter.x = (fX[0] + fX[1]) * 0.5f;
        vCenter.y = (fY[3] + fY[2]) * 0.5f;
        m_rtDrawList[6]->m_rtInitPos.Set(0, 0, fX[1] - fX[0], fY[3] - fY[2]);
        rt = pSprite->m_uvArray[6];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[6]->SetRectUV(trt);
        m_rtDrawList[6]->SetPosition(vCenter);

        vCenter.x = (fX[2] + fX[1]) * 0.5f;
        m_rtDrawList[7]->m_rtInitPos.Set(fX[1], fY[2], fX[2] - fX[1], fY[3] - fY[2]);
        rt = pSprite->m_uvArray[7];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[7]->SetRectUV(trt);
        m_rtDrawList[7]->SetPosition(vCenter);

        vCenter.x = (fX[3] + fX[2]) * 0.5f;
        m_rtDrawList[8]->m_rtInitPos.Set(fX[2], fY[2], fX[3] - fX[2], fY[3] - fY[2]);
        rt = pSprite->m_uvArray[8];
        trt.x1 = (float)rt.left;
        trt.y1 = (float)rt.top;
        trt.w = (float)rt.right;
        trt.h = (float)rt.bottom;
        m_rtDrawList[8]->SetRectUV(trt);
        m_rtDrawList[8]->SetPosition(vCenter);
        return true;
    }
    bool  TWidget::Release()
    {
        for (auto data : m_rtDrawList)
        {
            data->Release();
            delete data;
        }
        m_rtDrawList.clear();
        for (auto data : m_pChildList)
        {
            data->Release();
            delete data;
            data = nullptr;
        }
        m_pChildList.clear();
        return TObject2D::Release();
    }
    void  TWidget::AddChild(TWidget* pUI)
    {
        m_pChildList.push_back(pUI);
        pUI->m_pParentWidget = this;
    };
    void TWidget::SetSprite(TSprite* pSprite)
    {
        m_pSprite = pSprite;
    };
    TVector4D TWidget::GetSpriteOffsetUV()
    {
        TVector4D vOffset = { 0,0,1,1 };
        if (m_pSprite != nullptr && m_pSprite->m_uvArray.size())
        {
            RECT rt = m_pSprite->m_uvArray[0];
            vOffset.x = (float)rt.left / (float)m_ptImageSize.x;
            vOffset.y = (float)rt.top / (float)m_ptImageSize.y;
            vOffset.z = (float)(rt.left + rt.right) / (float)m_ptImageSize.x;
            vOffset.w = (float)(rt.top + rt.bottom) / (float)m_ptImageSize.y;
        }
        return vOffset;
    }
    bool  TWidget::Push(TWidget* btn) { return true; };
    bool  TWidget::Push(std::wstring name) { return true; };
    TRect TWidget::GetNextRect() { TRect rt; return rt; }

    void  TWidget::SetPostDrawList()
    {//
       ////for (auto data : m_pChildList)
       // {
       //     if (m_pPostDrawWidget.size())
       //     {
       //         m_pPostDrawWidget.insert(
       //             m_pPostDrawWidget.end(),
       //             m_pPostDrawWidget.begin(), m_pPostDrawWidget.end());
       //     }
       //     m_pPostDrawWidget.clear();
       // }

        if (m_pParentWidget&& m_pPostDrawWidget.size())
        {
            m_pParentWidget->m_pPostDrawWidget.insert(
                m_pParentWidget->m_pPostDrawWidget.end(),
                m_pPostDrawWidget.begin(), m_pPostDrawWidget.end());
            m_pPostDrawWidget.clear();
        }       
    }

    void  TWidget::PostDrawList()
    {       
       if (m_pParentWidget==nullptr)
        {           
            for (auto obj : m_pPostDrawWidget)
            {
                obj->Render();
                m_pSavePostDrawWidget.push_back(obj);
            }            
            m_pPostDrawWidget.clear();
        }       
    }
    void TWidget::Add(TPriorityMap& queue)
    {
        m_pPriorityQueue = &queue;
        auto data = std::make_pair(m_iPriority, this);
        m_Iterator = queue.insert(data);
    }

    void TWidget::Remove()
    {
        if (m_pPriorityQueue == nullptr || m_pPriorityQueue->size() <= 0) return;
        m_pPriorityQueue->erase(m_Iterator);
        m_pPriorityQueue = 0;
        m_Iterator = TPriorityMap::iterator();
    }
    int  TWidget::GetPriority() const 
    {
        return m_iPriority;
    }
    void TWidget::SetPriority(int iPriority)
    {
        if (m_pPriorityQueue == nullptr || m_pPriorityQueue->size() <= 0)
        {
            m_iPriority = iPriority;
            return;
        }
        m_iPriority = iPriority;
        if (m_pPriorityQueue)
        {
            TPriorityMap& queue = *m_pPriorityQueue;
            this->Remove();
            this->Add(queue);
        }
    }
    void TWidget::UpdateRects()
    {
        ::SetRect(&m_rcBoundingBox, m_rtCollision.x1, m_rtCollision.y1, m_rtCollision.x2, m_rtCollision.y2);
    }
    bool    TWidget::CanHaveFocus()
    {
        return false;
    }
    void    TWidget::OnFocusIn()
    {
        m_bHasFocus = true;
    }
    void    TWidget::OnFocusOut()
    {
        m_bHasFocus = false;
    }
    void    TWidget::OnMouseEnter()
    {
        m_bMouseOver = true;
    }
    void    TWidget::OnMouseLeave()
    {
        m_bMouseOver = false;
    }
    void   TWidget::OnHotkey()
    {
    }
    BOOL    TWidget::ContainsPoint(POINT pt)
    {
        return PtInRect(&m_rcBoundingBox, pt);
    }
    void    TWidget::SetEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
    }
    bool    TWidget::GetEnabled()
    {
        return m_bEnabled;
    }
    void    TWidget::SetVisible(bool bVisible)
    {
        m_bVisible = bVisible;
    }
    bool   TWidget::GetVisible()
    {
        return m_bVisible;
    }
    UINT    TWidget::GetType() const
    {
        return m_Type;
    }
    int     TWidget::GetID() const
    {
        return m_ID;
    }
    void    TWidget::SetID(int ID)
    {
        m_ID = ID;
    }
    void    TWidget::SetHotkey(UINT nHotkey)
    {
        m_nHotkey = nHotkey;
    }
    UINT    TWidget::GetHotkey()
    {
        return m_nHotkey;
    }
    void TWidget::Refresh()
    {
        m_bMouseOver = false;
        m_bHasFocus = false;
    }
 
    TWidget::TWidget()
    {
        m_ID = g_iNumCounter++;
        m_CurrentState = T_STATE_NORMAL;
        SetEnabled(true);
        SetVisible(true);
        m_pStateList.resize(T_STATE_MAXSTATE);
    }
    TWidget::TWidget(TSprite* pSprite) : TWidget()
    {
        m_pSprite = pSprite;
    }
};
