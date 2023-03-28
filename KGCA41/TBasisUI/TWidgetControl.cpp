#include "TWidgetControl.h"
#include "TBasisUI.h"
namespace TUI
{
    bool  TText::Init()
    {
        m_Type = T_CONTROL_STATIC;
        return true;
    }
    bool TText::SetAttribute(TRect rt, const std::vector<W_STR>& texStateList)
    {
        TWidget::SetAttribute( rt, texStateList);        
        //m_pTextLayoutSize = { m_rtCollision.w,m_rtCollision.h };
        SetText({ m_rtCollision.w,m_rtCollision.h }, m_szText);
        return true;
    }
    ComPtr<ID2D1StrokeStyle> TText::CreateStrokeStyle()
    {
        ComPtr<ID2D1StrokeStyle> strokeStyle;

        HRESULT hr = I_Writer.m_d2dFactory->CreateStrokeStyle(
            D2D1::StrokeStyleProperties(
                D2D1_CAP_STYLE_ROUND,
                D2D1_CAP_STYLE_ROUND,
                D2D1_CAP_STYLE_ROUND,
                D2D1_LINE_JOIN_ROUND,
                0.0f,
                D2D1_DASH_STYLE_SOLID,
                0.0f),
            nullptr,
            0,
            strokeStyle.GetAddressOf());
        return strokeStyle;
    }
    bool TText::Create(D2D1_RECT_F rt, const std::wstring fontName, UINT iFontSize, CONST D2D1_COLOR_F color)
    {
        /*if (!TWidget::Create(m_pd3dDevice, m_pImmediateContext, shadername, texturename))
        {
            TWidget::Release();
            return false;
        }*/
        m_pStrokeStyle = CreateStrokeStyle();
        m_rtCollision.Set(rt.left, rt.top, rt.right, rt.bottom);
        HRESULT hr = I_Writer.m_pDWriteFactory->CreateTextFormat(
            fontName.c_str(),
            NULL,
            DWRITE_FONT_WEIGHT_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            (FLOAT)iFontSize,
            L"en-us",//L"ko-kr",//L"en-us",
            m_pTextFormat.GetAddressOf());

        WCHAR fontFamilyName[64] = { 0, };
        m_pTextFormat->GetFontFamilyName(fontFamilyName, 64);

        m_TextColor = color;
        hr = I_Writer.m_d2dRT->CreateSolidColorBrush(m_TextColor, m_pTextColor.ReleaseAndGetAddressOf());
        if (SUCCEEDED(hr))
        {
            SetText({ m_rtCollision.w,m_rtCollision.h }, L"Text");
        }
        return true;
    }
    bool TText::Frame()
    {
        TWidget::Frame();
        SetPostDrawList();
        return true;
    }
    HRESULT TText::SetText(D2D1_POINT_2F size, std::wstring text, D2D1::ColorF Color)
    {        
        m_pTextLayoutSize = size;
        return SetText(text, Color);
    }
    HRESULT TText::SetText(std::wstring text, D2D1::ColorF Color)
    {
        HRESULT hr = S_OK;
        m_cTextLength = text.size();
        m_szText.clear();
        m_szText = text;
        hr = I_Writer.m_pDWriteFactory->CreateTextLayout(
            m_szText.c_str(),
            m_cTextLength,
            m_pTextFormat.Get(),
            m_pTextLayoutSize.x,
            m_pTextLayoutSize.y,
            m_pTextLayout.ReleaseAndGetAddressOf());
        if (SUCCEEDED(hr))
        {
            m_pTextLayout->GetMetrics(&m_TextMetrics);
        }
        return hr;
    }
    HRESULT TText::DrawTextLayout(D2D1_POINT_2F pos, D2D1::ColorF Color)
    {
        I_Writer.m_d2dRT->BeginDraw();
        //I_Writer.m_pTextColor->SetColor(Color);
        I_Writer.m_d2dRT->SetTransform(D2D1::Matrix3x2F::Identity());
        if (m_pTextLayout)
        {
            m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
        I_Writer.m_d2dRT->DrawTextLayout(pos, m_pTextLayout.Get(), m_pTextColor.Get());
        I_Writer.m_d2dRT->EndDraw();
        return S_OK;
    }
    bool  TText::Render()
    {
        DrawTextLayout({ m_rtCollision.x1, m_rtCollision.y1 });
        I_Writer.m_d2dRT->BeginDraw();
        D2D1_RECT_F rt = { m_rtCollision.x1, m_rtCollision.y1, m_rtCollision.x2,m_rtCollision.y2 };
        //I_Writer.m_pTextColor->SetColor(m_TextColor);
        //I_Writer.m_pTextColor->SetOpacity(1.0f);
        //I_Writer.m_pTextColor->SetTransform()
        //I_Writer.m_d2dRT->DrawText(m_szText.c_str(), m_szText.size(), m_pTextFormat, rt, I_Writer.m_pTextColor);
        I_Writer.m_d2dRT->DrawRectangle(rt, m_pTextColor.Get(), 1.0f, m_pStrokeStyle.Get());
        PostDrawList();
        I_Writer.m_d2dRT->EndDraw();        
        return true;
    }
    bool  TText::Release()
    {
        return TWidget::Release();
    }
    void  TText::SetRect(TRect rt)
    {
        m_rtInitPos = rt;
        if (m_pTexture)
        {
            m_ptImageSize.x = m_pTexture->m_Desc.Width;
            m_ptImageSize.y = m_pTexture->m_Desc.Height;
        }
        m_rtUV.x1 = 0.0f;
        m_rtUV.y1 = 0.0f;
        m_rtUV.w = 1.0f;
        m_rtUV.h = 1.0f;
    }
    void  TText::Move(TRect tRt)
    {
        SetAttribute(tRt);
    }

    bool  TButton::Init()
    {
        m_Type = T_CONTROL_BUTTON;
        return true;
    }
    bool  TButton::Frame()
    {
        TWidget::Frame();
        for (auto data : m_pChildList)
        {
            data->Frame();
        }
        SetPostDrawList();
        return true;
    }
    bool  TButton::Render()
    {
        if (m_pSprite)
        {
            for (int iDraw = 0; iDraw < m_pSprite->m_uvArray.size(); iDraw++)
            {
                RECT rt = m_pSprite->m_uvArray[iDraw];
                m_tRect.x1 = (float)rt.left;
                m_tRect.y1 = (float)rt.top;
                m_tRect.w = (float)rt.right;
                m_tRect.h = (float)rt.bottom;
                m_pSprite->SetRect(m_rtInitPos);
                m_pSprite->SetRectUV(m_tRect);
                m_pSprite->SetPosition(m_vPos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTextureSRV);
                if (m_pSprite->m_pMaskTex != nullptr)
                {
                    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSprite->m_pMaskTex->m_pTextureSRV);
                }
                m_pSprite->PostRender();
                if (!(m_CurrentState == T_STATE_PRESSED || m_CurrentState == T_STATE_HOLD))
                {
                    break;
                }
            }
        }
        else
        {
            TBaseObject::PreRender();
            m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
            TBaseObject::PostRender();
        }

        for (auto data : m_pChildList)
        {
            data->Render();
        }

        PostDrawList();
        return true;
    }
    bool  TButton::Release()
    {
        return TWidget::Release();
    }
    bool  TButton::SetAttribute(TRect rt, const std::vector<W_STR>& texStateList)
    {
        TWidget::SetAttribute( rt, texStateList);
        for (auto data : m_pChildList)
        {
            data->SetAttribute( rt, texStateList);
        }
        return true;
    }
    void  TButton::SetRect(TRect rt)
    {
        m_rtInitPos = rt;
        if (m_pTexture)
        {
            m_ptImageSize.x = m_pTexture->m_Desc.Width;
            m_ptImageSize.y = m_pTexture->m_Desc.Height;
        }
        m_rtUV.x1 = 0.0f;
        m_rtUV.y1 = 0.0f;
        m_rtUV.w = 1.0f;
        m_rtUV.h = 1.0f;
    }
    void  TButton::Move(TVector2D vOffsetPos)
    {
        TRect tRt = m_rtCollision;
        /*tRt += vOffsetPos;  */     
        SetAttribute({vOffsetPos.x, vOffsetPos.y, tRt.w, tRt.h});
        //Move(vOffsetPos);
        for (auto data : m_pChildList)
        {
            data->Move(vOffsetPos);
        }
    }
    void  TButton::Move(TRect tRt)
    {
        SetAttribute(tRt);
        for (auto data : m_pChildList)
        {
            data->Move(tRt);
        }
    }

    bool  TListControl::Init()
    {
        m_Type = T_CONTROL_LISTBOX;
        return true;
    }
    bool  TListControl::Frame()
    {
        TWidget::Frame();
        for (auto data : m_pChildList)
        {
            TVector2D pos = data->m_vPos + m_vOffsetPos;
            data->SetPosition(pos);
            data->Frame();
        }
        SetPostDrawList();
        return true;
    }
    bool  TListControl::Render()
    {
        TBaseObject::PreRender();
        if (m_pCurrentTex)
        {
            m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
        }
        TBaseObject::PostRender();

        for (auto data : m_pChildList)
        {
            data->Render();
        }
        PostDrawList();
        return true;
    }
    bool  TListControl::Release()
    {
        return TWidget::Release();
    }
    void  TListControl::SetRect(TRect rt)
    {
        m_rtInitPos = rt;
        if (m_pTexture)
        {
            m_ptImageSize.x = m_pTexture->m_Desc.Width;
            m_ptImageSize.y = m_pTexture->m_Desc.Height;
        }
        m_rtUV.x1 = 0.0f;
        m_rtUV.y1 = 0.0f;
        m_rtUV.w = 1.0f;
        m_rtUV.h = 1.0f;
    }

    bool  TListBox::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        for (auto data : m_pChildList)
        {
            data->MsgProc(uMsg, wParam, lParam);
        }
        return false;
    }
    bool  TListBox::Init()
    {
        m_Type = T_CONTROL_LISTBOX;
        return true;
    }
    bool  TListBox::Frame()
    {
        m_VisibleList.clear();
        TWidget* pScrollBar = m_pChildList[1];
        pScrollBar->Frame();

        TWidget* pPanel = m_pChildList[0];
        if (pPanel->m_pChildList.size() > 0)
        {
            int iMaxButtonCounter = (int)((pPanel->m_rtCollision.h) / (float)(pPanel->m_pChildList[0]->m_rtCollision.h));
            int iMaxButtonIndex = (int)(pPanel->m_pChildList.size() - iMaxButtonCounter);
            float fMiddleValue = pPanel->m_pChildList.size() * pScrollBar->m_fPercentage;
            m_iDragBegin = (int)max(0, min(iMaxButtonIndex, fMiddleValue - (iMaxButtonCounter / 2.0f)));
            m_iDragEnd = (int)min(pPanel->m_pChildList.size(), m_iDragBegin + iMaxButtonCounter);
            for (int iBtn = m_iDragBegin; iBtn < m_iDragEnd; iBtn++)
            {
                m_VisibleList.push_back(pPanel->m_pChildList[iBtn]);
            }

            /*if (m_pParentWidget == nullptr)
            {
                for (auto select : m_pSavePostDrawWidget)
                {
                    if (select == this) break;
                    select->Frame();
                    if (select->m_CurrentState == T_STATE_SELECT)
                    {
                        m_pSelectBtn = select;
                        break;
                    }
                }
                m_pSavePostDrawWidget.clear();
            }*/

            if (m_pPostDrawWidget.empty())
            {
                for (auto data : m_VisibleList)
                {
                    data->Frame();
                    if (data->m_CurrentState == T_STATE_SELECT)
                    {
                        m_pSelectBtn = data;
                        break;
                    }
                    if (data->m_pParentWidget && !data->m_pParentWidget->m_pPostDrawWidget.empty())
                    {
                        break;
                    }
                }
            }

            pPanel->Frame();

            SetPostDrawList();
        }
        return true;
    }
    bool  TListBox::Render()
    {
        for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
        {
            m_rtDrawList[iSub]->Render();
        }
       /* TWidget* pButtonListBG = m_pChildList[0];
        pButtonListBG->Render();*/
        for (int iBtn = 0; iBtn < m_VisibleList.size(); iBtn++)
        {
           /* m_VisibleList[iBtn]->SetAttribute({ 
                m_rtCollision.x1, 
                m_rtCollision.y1+ m_fBtnHeight * iBtn , 
                m_VisibleList[iBtn]->m_rtCollision.w, 
                m_fBtnHeight });*/
            TVector2D vPos = { m_rtCollision.x1,
                                m_rtCollision.y1 + m_fBtnHeight * iBtn };
            TRect rt = { m_rtCollision.x1,
                        m_rtCollision.y1 + m_fBtnHeight * iBtn ,
                        m_VisibleList[iBtn]->m_rtCollision.w,
                        m_fBtnHeight };
            m_VisibleList[iBtn]->Move(rt);
            m_VisibleList[iBtn]->Render();
        }
        TWidget* pScrollBar = m_pChildList[1];
        pScrollBar->Render();      

        PostDrawList();
        return true;
    }
    bool  TListBox::Release()
    {
        return TWidget::Release();
    }
    void  TListBox::SetRect(TRect rt)
    {
        m_rtInitPos = rt;
        if (m_pTexture)
        {
            m_ptImageSize.x = m_pTexture->m_Desc.Width;
            m_ptImageSize.y = m_pTexture->m_Desc.Height;
        }
        m_rtUV.x1 = 0.0f;
        m_rtUV.y1 = 0.0f;
        m_rtUV.w = 1.0f;
        m_rtUV.h = 1.0f;
    }
    TWidget* TListBox::Push(TWidget* btn)
    {
        TWidget* pPannel = m_pChildList[0];
        btn->SetAttribute({ pPannel->m_rtCollision.x1,
            pPannel->m_rtCollision.y1 + m_fBtnHeight * pPannel->m_pChildList.size(),
            pPannel->m_rtCollision.w,
            m_fBtnHeight });
        pPannel->AddChild(btn);
        return btn;
    }
    TWidget* TListBox::Push(std::wstring name)
    {
        TWidget* pPannel = m_pChildList[0];
        std::wstring text = name;
        //text += std::to_wstring(pPannel->m_pChildList.size());
        TWidget* btn = CreateButton(m_pd3dDevice, m_pImmediateContext, I_Sprite.GetPtr(L"DXListBox"),
            { m_rtCollision.x1, m_rtCollision.y1 + m_fBtnHeight * pPannel->m_pChildList.size(),
                m_rtCollision.w, m_fBtnHeight }, text.c_str());
        pPannel->AddChild(btn);        
        return btn;
    }
    TRect TListBox::GetNextRect()
    {
        TWidget* pListBtn = m_pChildList[0];
        TRect rt;
        rt.Set(m_rtCollision.x1, m_rtCollision.y1 + m_fBtnHeight * pListBtn->m_pChildList.size(),
            m_rtCollision.w, m_fBtnHeight);
        return rt;
    }
    void  TListBox::Move(TVector2D vOffsetPos)
    {
        TRect tRt = m_rtCollision;
        //tRt += vOffsetPos;
        // 전체크기 = 버튼 너비 + 스크롤바 너비( 스크롤바 버튼은 1개 버튼의 크기로 한다.)
        SetAttribute({vOffsetPos.x, vOffsetPos.y, tRt.w, tRt.h});
        //Move(vOffsetPos);
        TWidget* pPanel = (TPanel*)m_pChildList[0];
        pPanel->SetAttribute({ m_rtCollision.x1, m_rtCollision.y1, m_rtCollision.w, m_rtCollision.h });
        //pRootCtl->AddChild(pPanel);

        // 버튼들의 간격은 없다.
        for (UINT iBtn = 0; iBtn < pPanel->m_pChildList.size(); iBtn++)
        {
            TButton* btn = (TButton*)pPanel->m_pChildList[iBtn];                
            btn->SetAttribute({ pPanel->m_rtCollision.x1, pPanel->m_rtCollision.y1 + m_fBtnHeight * iBtn, pPanel->m_rtCollision.w - iBtn, m_fBtnHeight });
            //pPanel->AddChild(btn);
        }

        float fMaxScrollBtnSizeX = min(30.0f, m_fBtnWidth);
        float fMaxScrollBtnSizeY = min(30.0f, m_fBtnHeight);
        TScrollBar* pScrollBar = (TScrollBar*)m_pChildList[1];
        pScrollBar->SetAttribute({ m_rtCollision.x2, m_rtCollision.y1, fMaxScrollBtnSizeX, m_rtCollision.h });
        //pRootCtl->AddChild(pScrollBar);

        TButton* pUpBtn = (TButton*)m_pChildList[2];
        pUpBtn->SetAttribute({ m_rtCollision.x2, m_rtCollision.y1, fMaxScrollBtnSizeX, fMaxScrollBtnSizeY });
        //pScrollBar->AddChild(pUpBtn);

        TButton* pDownBtn = (TButton*)m_pChildList[3];       
        pDownBtn->SetAttribute({ m_rtCollision.x2, m_rtCollision.y2 - fMaxScrollBtnSizeY, fMaxScrollBtnSizeX, fMaxScrollBtnSizeY });
        //pScrollBar->AddChild(pDownBtn);

        TButton* pDragBtn = (TButton*)m_pChildList[4];      
        pDragBtn->SetAttribute({ m_rtCollision.x2, m_rtCollision.cy, fMaxScrollBtnSizeX, fMaxScrollBtnSizeY });
       // pScrollBar->AddChild(pDragBtn);
    }
    void  TListBox::Move(TRect tRt)
    {       
        SetAttribute(tRt);
        SetDrawList(0.1f, 0.1f, 0.1f, 0.1f, 0.2f, 0.2f, 0.33f, 0.33f);
        TWidget* pPanel = (TPanel*)m_pChildList[0];
        pPanel->Move({ tRt.x1,  tRt.y1, max(10.0f, tRt.w - m_fBtnWidth), tRt.h });
        // 버튼들의 간격은 없다.
        for (UINT iBtn = 0; iBtn < pPanel->m_pChildList.size(); iBtn++)
        {
            TButton* btn = (TButton*)pPanel->m_pChildList[iBtn];
            btn->Move({ pPanel->m_rtCollision.x1,
                                pPanel->m_rtCollision.y1 + m_fBtnHeight * iBtn, 
                                pPanel->m_rtCollision.w - iBtn, 
                                m_fBtnHeight });
        }


        TScrollBar* pScrollBar = (TScrollBar*)m_pChildList[1];
        pScrollBar->Move({ pPanel->m_rtCollision.x2, m_rtCollision.y1, m_fMaxScrollBtnSizeX, m_rtCollision.h });

        /*TButton* pUpBtn = (TButton*)pScrollBar->m_pChildList[0];
        pUpBtn->Move({ pPanel->m_rtCollision.x2, m_rtCollision.y1, m_fMaxScrollBtnSizeX, m_fMaxScrollBtnSizeY });

        TButton* pDownBtn = (TButton*)pScrollBar->m_pChildList[1];
        pDownBtn->Move({ pPanel->m_rtCollision.x2, m_rtCollision.y2 - m_fMaxScrollBtnSizeY, m_fMaxScrollBtnSizeX, m_fMaxScrollBtnSizeY });

        TButton* pDragBtn = (TButton*)pScrollBar->m_pChildList[2];
        pDragBtn->Move({ pPanel->m_rtCollision.x2, pDragBtn->m_vPos.y, m_fMaxScrollBtnSizeX, m_fMaxScrollBtnSizeY });*/
    }

    bool  TCheckBtn::Init()
    {
        m_Type = T_CONTROL_CHECKBOX;
        return true;
    }
    bool  TCheckBtn::Frame()
    {
        POINT ptMouse = I_InputUI.m_ptPos;
        if (TCollision::RectToPoint(m_rtCollision, ptMouse))
        {
            m_CurrentState = T_STATE_MOUSEOVER;
            m_pCurrentTex = m_pStateList[T_STATE_MOUSEOVER];
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH ||
                I_InputUI.GetKey(VK_LBUTTON) == KEY_HOLD)
            {
                m_CurrentState = T_STATE_PRESSED;
                m_pCurrentTex = m_pStateList[T_STATE_PRESSED];
            }
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_UP)
            {
                m_CurrentState = T_STATE_SELECT;
                m_bToggle = !m_bToggle;
                if (m_bToggle)
                {
                    RECT rt = m_pSprite->m_uvArray[1];
                    m_tRect.x1 = (float)rt.left;
                    m_tRect.y1 = (float)rt.top;
                    m_tRect.w = (float)rt.right;
                    m_tRect.h = (float)rt.bottom;
                }
                else
                {
                    RECT rt = m_pSprite->m_uvArray[0];
                    m_tRect.x1 = (float)rt.left;
                    m_tRect.y1 = (float)rt.top;
                    m_tRect.w = (float)rt.right;
                    m_tRect.h = (float)rt.bottom;
                }
            }
        }
        else
        {
            m_pCurrentTex = m_pStateList[T_STATE_NORMAL];
        }
        SetPostDrawList();
        return true;
    }
    bool  TCheckBtn::Render()
    {
        if (m_pSprite)
        {
            for (int iDraw = 0; iDraw < m_pSprite->m_uvArray.size(); iDraw++)
            {
                RECT rt = m_pSprite->m_uvArray[iDraw];
                m_tRect.x1 = (float)rt.left;
                m_tRect.y1 = (float)rt.top;
                m_tRect.w = (float)rt.right;
                m_tRect.h = (float)rt.bottom;
                m_pSprite->SetRect(m_rtInitPos);
                m_pSprite->SetRectUV(m_tRect);
                m_pSprite->SetPosition(m_vPos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTextureSRV);
                if (m_pSprite->m_pMaskTex != nullptr)
                {
                    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSprite->m_pMaskTex->m_pTextureSRV);
                }
                m_pSprite->PostRender();
                if (!m_bToggle) break;
            }
        }
        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Render();
        }
        PostDrawList();
        return true;
    }
    bool  TCheckBtn::Release()
    {
        return TWidget::Release();
    }
    bool  TRadioBtn::Init()
    {
        m_Type = T_CONTROL_RADIOBUTTON;
        return true;
    }
    bool  TRadioBtn::Frame()
    {
        SetPostDrawList();
        return true;
    }
    bool  TRadioBtn::Render()
    {
        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Render();
        }
        PostDrawList();
        return true;
    }
    bool  TRadioBtn::Release()
    {
        return TWidget::Release();
    }

    bool  TSlider::Init()
    {
        m_Type = T_CONTROL_SLIDER;
        return true;
    }
    bool  TSlider::Frame()
    {
        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            TWidget* pChild = m_pChildList[iChild];
            if (pChild->Frame())
            {
                POINT move = { 0, };
                if (pChild->m_CurrentState == T_STATE_PRESSED)
                {
                    m_bMouseDrag = true;
                    m_ptMouseDragStart = I_InputUI.m_ptPos;
                }
                if (pChild->m_CurrentState == T_STATE_HOLD)
                {
                    move.x = I_InputUI.m_ptPos.x - m_ptMouseDragStart.x;
                    move.y = I_InputUI.m_ptPos.y - m_ptMouseDragStart.y;

                    // 독립적으로 사용
                    TVector2D vPos = pChild->m_vPos;
                    vPos.x += move.x;
                    vPos.x = max(m_rtCollision.x1 + m_fSliderDragBtnWidth/2.0f, vPos.x);
                    vPos.x = min(m_rtCollision.x2 - m_fSliderDragBtnWidth/2.0f, vPos.x);
                    pChild->SetPosition(vPos);

                    // 리스트에서 사용
                    pChild->m_vOffset.x += move.x;
                    pChild->m_vOffset.x = max(0, pChild->m_vOffset.x);
                    pChild->m_vOffset.x = min(m_rtCollision.w - m_fSliderDragBtnWidth, pChild->m_vOffset.x);   
                    m_fPercentage = smoothstep(pChild->m_vOffset.x, 0.0f, (float)(m_rtCollision.w - m_fSliderDragBtnWidth));
                    m_ptMouseDragStart = I_InputUI.m_ptPos;
                }
                if (pChild->m_CurrentState == T_STATE_SELECT)
                {
                    m_bMouseDrag = false;
                }
            }
        }
        SetPostDrawList();
        return true;
    }
    bool  TSlider::Render()
    {
        if (m_pSprite)
        {
            for (int iDraw = 0; iDraw < m_pSprite->m_uvArray.size(); iDraw++)
            {
                RECT rt = m_pSprite->m_uvArray[iDraw];
                m_tRect.x1 = (float)rt.left;
                m_tRect.y1 = (float)rt.top;
                m_tRect.w = (float)rt.right;
                m_tRect.h = (float)rt.bottom;
                m_pSprite->SetRect(m_rtInitPos);
                m_pSprite->SetRectUV(m_tRect);
                m_pSprite->SetPosition(m_vPos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTexture->m_pTextureSRV);
                if (m_pSprite->m_pMaskTex != nullptr)
                {
                    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSprite->m_pMaskTex->m_pTextureSRV);
                }
                m_pSprite->PostRender();
                if (!m_bToggle) break;
            }
        }
        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Render();
        }
        PostDrawList();
        return true;
    }
    bool  TSlider::Release()
    {
        return TWidget::Release();
    }
    void  TSlider::Move(TRect tRt)
    {
        SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });

        TWidget* pSliderButton = m_pChildList[0];
        pSliderButton->Move({ tRt.x1+ pSliderButton->m_vOffset.x,  tRt.y1, m_fSliderDragBtnWidth, m_fSliderDragBtnHeight });
    }

    bool  TScrollBar::Init()
    {
        m_Type = T_CONTROL_SCROLLBAR;
        return true;
    }
    bool  TScrollBar::Frame()
    {
        TWidget* pUpArraw = m_pChildList[0];
        TWidget* pDownArraw = m_pChildList[1];
        TWidget* pScrollBtn = m_pChildList[2];
        for (auto data : m_pChildList)
        {
            data->Frame();
        }
        // up array
        if (pUpArraw->m_CurrentState == T_STATE_SELECT ||
            pUpArraw->m_CurrentState == T_STATE_HOLD)
        {
            POINT move = { 0, };
            TVector2D vPos = pScrollBtn->m_vPos;
            vPos.y -= g_fSecondPerFrame * 100.0f;

            float fMinY = pUpArraw->m_rtCollision.y2 + (pScrollBtn->m_rtCollision.h / 2);
            float fMaxY = pDownArraw->m_rtCollision.y1 - (pScrollBtn->m_rtCollision.h / 2);
            vPos.y = max(fMinY, vPos.y);
            vPos.y = min(fMaxY, vPos.y);
            m_fPercentage = (vPos.y - fMinY) / (fMaxY - fMinY);
            if (pScrollBtn->m_bMovement)
            {
                pScrollBtn->SetPosition(vPos);
            }
        }
        // down 
        if (pDownArraw->m_CurrentState == T_STATE_SELECT ||
            pDownArraw->m_CurrentState == T_STATE_HOLD)
        {
            POINT move = { 0, };
            TVector2D vPos = pScrollBtn->m_vPos;
            vPos.y += g_fSecondPerFrame * 100.0f;
            float fMinY = pUpArraw->m_rtCollision.y2 + (pScrollBtn->m_rtCollision.h / 2);
            float fMaxY = pDownArraw->m_rtCollision.y1 - (pScrollBtn->m_rtCollision.h / 2);
            vPos.y = max(fMinY, vPos.y);
            vPos.y = min(fMaxY, vPos.y);
            m_fPercentage = (vPos.y - fMinY) / (fMaxY - fMinY);
            if (pScrollBtn->m_bMovement)
            {
                pScrollBtn->SetPosition(vPos);
            }
        }
        // scrool button 
        POINT move = { 0, };
        pScrollBtn->m_bToggle = false;
        if (pScrollBtn->m_CurrentState == T_STATE_PRESSED)
        {
            m_bMouseDrag = true;
            m_ptMouseDragStart = I_InputUI.m_ptPos;
        }
        if (pScrollBtn->m_CurrentState == T_STATE_HOLD)
        {
            move.x = I_InputUI.m_ptPos.x - m_ptMouseDragStart.x;
            move.y = I_InputUI.m_ptPos.y - m_ptMouseDragStart.y;
            TVector2D vPos = pScrollBtn->m_vPos;
            //vPos.x += move.x;
            vPos.y += move.y;
            // vPos.x = max(m_rtCollision.x1/*+ pChild->m_rtCollision.w/2*/, vPos.x);
            // vPos.x = min(m_rtCollision.x2/*- pChild->m_rtCollision.w/2*/, vPos.x);
            float fMinY = pUpArraw->m_rtCollision.y2 + (pScrollBtn->m_rtCollision.h / 2);
            float fMaxY = pDownArraw->m_rtCollision.y1 - (pScrollBtn->m_rtCollision.h / 2);
            vPos.y = max(fMinY, vPos.y);
            vPos.y = min(fMaxY, vPos.y);
            m_fPercentage = (vPos.y - fMinY) / (fMaxY - fMinY);
            if (pScrollBtn->m_bMovement)
            {
                pScrollBtn->SetPosition(vPos);
            }
            m_ptMouseDragStart = I_InputUI.m_ptPos;
        }
        if (pScrollBtn->m_CurrentState == T_STATE_SELECT)
        {
            m_bMouseDrag = false;
            pScrollBtn->m_bToggle = true;
        }

        SetPostDrawList();
        return true;
    }
    bool  TScrollBar::Render()
    {
        if (m_pSprite)
        {
            for (int iDraw = 0; iDraw < m_pSprite->m_uvArray.size(); iDraw++)
            {
                RECT rt = m_pSprite->m_uvArray[iDraw];
                m_tRect.x1 = (float)rt.left;
                m_tRect.y1 = (float)rt.top;
                m_tRect.w = (float)rt.right;
                m_tRect.h = (float)rt.bottom;
                m_pSprite->SetRect(m_rtInitPos);
                m_pSprite->SetRectUV(m_tRect);
                m_pSprite->SetPosition(m_vPos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTexture->m_pTextureSRV);
                if (m_pSprite->m_pMaskTex != nullptr)
                {
                    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSprite->m_pMaskTex->m_pTextureSRV);
                }
                m_pSprite->PostRender();
                if (m_bToggle) break;
            }
        }
        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Render();
        }
        PostDrawList();
        return true;
    }
    bool  TScrollBar::Release()
    {
        return TWidget::Release();
    }
    void  TScrollBar::Move(TRect tRt)
    {
        SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });
        /**/
        TButton* pUpArraw = (TButton*)m_pChildList[0];
        pUpArraw->Move({ m_rtCollision.x1, m_rtCollision.y1, m_fMaxScrollBtnSizeX, m_fMaxScrollBtnSizeY });

       TButton* pDownArraw = (TButton*)m_pChildList[1];
       pDownArraw->Move({ m_rtCollision.x1, m_rtCollision.y2 - m_fMaxScrollBtnSizeY, m_fMaxScrollBtnSizeX, m_fMaxScrollBtnSizeY });


       float fMinY = pUpArraw->m_rtCollision.y2;
       float fMaxY = pDownArraw->m_rtCollision.y1 - m_fMaxScrollBtnSizeY;
       //m_fPercentage = (vPos.y - fMinY) / (fMaxY - fMinY);
       float fCurrentY = fMinY + (fMaxY - fMinY) * m_fPercentage;
       TButton* pDragBtn = (TButton*)m_pChildList[2];
       pDragBtn->Move({ m_rtCollision.x1, fCurrentY, m_fMaxScrollBtnSizeX, m_fMaxScrollBtnSizeY });
    }

    bool  TProgressBar::Init()
    {
        m_Type = T_CONTROL_PROGRESSBAR;
        return true;
    }
    bool  TProgressBar::Frame()
    {
        TWidget::Frame();
        for (auto data : m_pChildList)
        {
            data->Frame();
        }
        SetPostDrawList();
        return true;
    }
    bool  TProgressBar::Render()
    {
        if (m_pSprite)
        {
            for (int iDraw = 0; iDraw < m_pSprite->m_uvArray.size(); iDraw++)
            {
                RECT rt = m_pSprite->m_uvArray[iDraw];
                m_tRect.x1 = (float)rt.left;
                m_tRect.y1 = (float)rt.top;
                m_tRect.w = (float)rt.right;
                m_tRect.h = (float)rt.bottom;
                m_pSprite->SetRect(m_rtInitPos);
                m_pSprite->SetRectUV(m_tRect);
                m_pSprite->SetPosition(m_vPos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTextureSRV);
                if (m_pSprite->m_pMaskTex != nullptr)
                {
                    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSprite->m_pMaskTex->m_pTextureSRV);
                }
                m_pSprite->PostRender();
                if (!(m_CurrentState == T_STATE_PRESSED || m_CurrentState == T_STATE_HOLD))
                {
                    break;
                }
            }
            ////////////////////////////////////// background //////////////////////////////
            if ( m_pSprite->m_pTexArray.size() > 0)
            {
                m_pSprite->m_rtInitPos = m_rtInitPos;
                m_pSprite->SetRectUV({0,0, (float)m_pSprite->m_ptImageSize.x, (float)m_pSprite->m_ptImageSize .y});
                m_pSprite->SetPosition(m_vPos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTextureSRV);
                if (m_pSprite->m_pMaskTex != nullptr)
                {
                    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSprite->m_pMaskTex->m_pTextureSRV);
                }
                m_pSprite->PostRender();                
                ////////////////////////////////// progress //////////////////////////////////////
                m_pSprite->m_rtInitPos.x1 = m_rtInitPos.x1;
                m_pSprite->m_rtInitPos.y1 = m_rtInitPos.y1;
                m_pSprite->m_rtInitPos.w = m_rtInitPos.w * (cosf(g_fGameTimer)*0.5f+0.5f);
                m_pSprite->m_rtInitPos.h = m_rtInitPos.h;// *cosf(g_fGameTimer);
                m_pSprite->SetRectUV({ 0,0, (float)m_pSprite->m_ptImageSize.x, (float)m_pSprite->m_ptImageSize.y });
                TVector2D pos = m_vPos;
                pos.x = m_pSprite->m_rtInitPos.x1;
                pos.x += m_pSprite->m_rtInitPos.w/2;
                m_pSprite->SetPosition(pos);
                m_pSprite->PreRender();
                m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSprite->m_pTexArray[1]->m_pTextureSRV);
                m_pSprite->PostRender();
            }
        }
        else
        {
            TBaseObject::PreRender();
            m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
            TBaseObject::PostRender();
        }
       /* for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Render();
        }*/
        PostDrawList();
        return true;
    }
    bool  TProgressBar::Release()
    {
        return TWidget::Release();
    }
    void  TProgressBar::Move(TRect tRt)
    {
        //SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });

        /*TWidget* pSliderButton = m_pChildList[0];
        pSliderButton->Move({ tRt.x1 + pSliderButton->m_vOffset.x,  tRt.y1, m_fSliderDragBtnWidth, m_fSliderDragBtnHeight });*/
    }


    bool  TPanel::Init()
    {
        return true;
    }
    bool  TPanel::Render()
    {       
        PostDrawList();
        return true;
    }
    bool  TPanel::Frame()
    {
        SetPostDrawList();
        return true;
    }
    bool  TPanel::Release()
    {
        return TWidget::Release();
    }


    bool  TDialog::Init()
    {
        m_Type = T_DIGALOG;
        m_ID = 0;
        m_bEnabled = true;
        m_bVisible = true;
        m_bMouseOver = false;
        m_bHasFocus = false;
        m_bIsDefault = false;
        ZeroMemory(&m_rcBoundingBox, sizeof(m_rcBoundingBox));
        return true;
    }
    bool  TDialog::Render()
    {
        m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pImmediateContext->RSSetState(TUI::TDxStateUI::g_pUI_SolidRS);
        m_pImmediateContext->OMSetBlendState(TUI::TDxStateUI::g_pUI_AlphaBlend, 0, -1);

        for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
        {
            m_rtDrawList[iSub]->Render();
        }
        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Render();
        }       
        PostDrawList();
        return true;
    }
    bool  TDialog::Frame()
    {
        POINT ptMouse = I_InputUI.m_ptPos;
        if (TCollision::RectToPoint(m_rtCollision, ptMouse))
        {
            m_CurrentState = T_STATE_MOUSEOVER;
            m_pCurrentTex = m_pStateList[T_STATE_MOUSEOVER];
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH ||
                I_InputUI.GetKey(VK_LBUTTON) == KEY_HOLD)
            {
                m_CurrentState = T_STATE_PRESSED;
                m_pCurrentTex = m_pStateList[T_STATE_PRESSED];
            }
            if (I_InputUI.GetKey(VK_LBUTTON) == KEY_UP)
            {
                m_CurrentState = T_STATE_SELECT;
            }
        }
        else
        {
            m_pCurrentTex = m_pStateList[T_STATE_NORMAL];
        }

        for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
        {
            m_pChildList[iChild]->Frame();
        }

        SetPostDrawList();
        return true;
    }
    bool  TDialog::Release()
    {
        return TWidget::Release();
    }

};

namespace TUI
{
    TText* CreateText(D2D1_RECT_F rt, const std::wstring fontName, UINT iFontSize, CONST D2D1_COLOR_F color)
    {
        TText* pTextBtn = new TText();
        pTextBtn->Create(rt, fontName, iFontSize,color);
        return pTextBtn;
    }
    TListBox* CreateListBox(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        UINT iNumVisibleBtn, UINT iNumBtn,
        TRect tRt)
    {
        float fBtnHeight = tRt.h / (float)iNumVisibleBtn;// 1개 버튼 높이
        float fBtnWidth = fBtnHeight;// 1개 버튼 넓이

        TListBox* pRootCtl = new TListBox(pSprite);
        pRootCtl->m_fBtnWidth = fBtnWidth;
        pRootCtl->m_fBtnHeight = fBtnHeight;
        pRootCtl->m_INumVisibleBtn = iNumVisibleBtn;
        pRootCtl->m_INumButtonCount = iNumBtn;
        
        pRootCtl->Create(pd3dDevice, pImmediateContext, pRootCtl->m_pSprite->m_pShader, pRootCtl->m_pSprite->m_pTexture);
        // 전체크기 = 버튼 너비 + 스크롤바 너비( 스크롤바 버튼은 1개 버튼의 크기로 한다.)
        pRootCtl->SetAttribute({ tRt.x1,  tRt.y1, max(10.0f, tRt.w - fBtnWidth), tRt.h });
        pRootCtl->SetDrawList(0.1f, 0.1f, 0.1f, 0.1f, 0.2f, 0.2f, 0.33f, 0.33f);

        TWidget* pPanel = new TPanel(I_Sprite.GetPtr(L"DXScrollBar"));
        pPanel->Create(pd3dDevice, pImmediateContext, pPanel->m_pSprite->m_pShader, pPanel->m_pSprite->m_pTexture);
        pPanel->SetAttribute({ pRootCtl->m_rtCollision.x1, pRootCtl->m_rtCollision.y1, pRootCtl->m_rtCollision.w, pRootCtl->m_rtCollision.h });
        pRootCtl->AddChild(pPanel);

        // 버튼들의 간격은 없다.
        for (UINT iBtn = 0; iBtn < iNumBtn; iBtn++)
        {
            std::wstring text = L"item";
            text += std::to_wstring(iBtn);
            TWidget* btn = CreateButton(pd3dDevice, pImmediateContext, I_Sprite.GetPtr(L"DXListBox"),
                { pPanel->m_rtCollision.x1, pPanel->m_rtCollision.y1+ fBtnHeight*iBtn,
                  pPanel->m_rtCollision.w - iBtn, fBtnHeight }, text.c_str());
            pPanel->AddChild(btn);
        }

        pRootCtl->m_fMaxScrollBtnSizeX = min(30.0f, fBtnWidth);
        pRootCtl->m_fMaxScrollBtnSizeY = min(30.0f, fBtnHeight);
        TScrollBar* pScrollBar = new TScrollBar(I_Sprite.GetPtr(L"DXScrollBar"));
        pScrollBar->Create(pd3dDevice, pImmediateContext, pScrollBar->m_pSprite->m_pShader, pScrollBar->m_pSprite->m_pTexture);
        pScrollBar->SetAttribute({ pRootCtl->m_rtCollision.x2, pRootCtl->m_rtCollision.y1, pRootCtl->m_fMaxScrollBtnSizeX, tRt.h });
        pScrollBar->m_fMaxScrollBtnSizeX = min(30.0f, fBtnWidth);
        pScrollBar->m_fMaxScrollBtnSizeY = min(30.0f, fBtnHeight);
        pRootCtl->AddChild(pScrollBar);

        TButton* pUpBtn = new TButton(I_Sprite.GetPtr(L"DXScrollBarUpButton"));
        pUpBtn->Create(pd3dDevice, pImmediateContext, pUpBtn->m_pSprite->m_pShader, pUpBtn->m_pSprite->m_pTexture);
        pUpBtn->m_bMovement = false;
        pUpBtn->SetAttribute({ pRootCtl->m_rtCollision.x2, pRootCtl->m_rtCollision.y1, pRootCtl->m_fMaxScrollBtnSizeX, pRootCtl->m_fMaxScrollBtnSizeY });
        pScrollBar->AddChild(pUpBtn);

        TButton* pDownBtn = new TButton(I_Sprite.GetPtr(L"DXScrollBarDownButton"));
        pDownBtn->Create(pd3dDevice, pImmediateContext, pDownBtn->m_pSprite->m_pShader, pDownBtn->m_pSprite->m_pTexture);
        pDownBtn->m_bMovement = false;
        pDownBtn->SetAttribute({ pRootCtl->m_rtCollision.x2, pRootCtl->m_rtCollision.y2 - pRootCtl->m_fMaxScrollBtnSizeY, pRootCtl->m_fMaxScrollBtnSizeX, pRootCtl->m_fMaxScrollBtnSizeY });
        pScrollBar->AddChild(pDownBtn);

        TButton* pDragBtn = new TButton(I_Sprite.GetPtr(L"DXScrollBarDragButtom"));
        pDragBtn->Create(pd3dDevice, pImmediateContext, pDragBtn->m_pSprite->m_pShader, pDragBtn->m_pSprite->m_pTexture);
        pDragBtn->SetAttribute({ pRootCtl->m_rtCollision.x2, pRootCtl->m_rtCollision.y1+pRootCtl->m_fMaxScrollBtnSizeY, pRootCtl->m_fMaxScrollBtnSizeX, pRootCtl->m_fMaxScrollBtnSizeY });
        pScrollBar->AddChild(pDragBtn);

        return pRootCtl;
    }

    TCheckBtn* CreateCheckButton(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt)
    {
        TCheckBtn* pRootCtl = new TCheckBtn(pSprite);
        pRootCtl->Create(pd3dDevice, pImmediateContext);
        pRootCtl->SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });
        //pRootCtl->SetSprite(I_Sprite.GetPtr(L"DXCheckButton"));


        D2D1_POINT_2F size = I_Writer.GetTextSize(L"Check", L"Elephant", 15);
        size.x += 10;
        TText* pText = new TText();
        pText->Create({ pRootCtl->m_rtCollision.x2,pRootCtl->m_rtCollision.y1,size.x,pRootCtl->m_rtCollision.h }, L"Elephant", 15);
        pText->SetText({ size.x,pRootCtl->m_rtCollision.h }, L"Check");
        pRootCtl->AddChild(pText);
        return pRootCtl;
    }

    TButton* CreateButton(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt, std::wstring text)
    {
        TButton* pRootCtl = new TButton(pSprite);
        pRootCtl->m_szName = text;
        pRootCtl->Create(pd3dDevice, pImmediateContext);
        pRootCtl->SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });
        //pRootCtl->SetSprite(I_Sprite.GetPtr(L"DXCheckButton"));

        //D2D1_POINT_2F size = I_Writer.GetTextSize(text.c_str(), L"Elephant", tRt.h / 3);
        TText* pText = new TText();
        pText->Create({ pRootCtl->m_rtCollision.x1,pRootCtl->m_rtCollision.y1,pRootCtl->m_rtCollision.w,pRootCtl->m_rtCollision.h }, L"Elephant", 
            (tRt.w < tRt.h) ? tRt.w/ 3 : tRt.h / 3);
        pText->SetText({ pRootCtl->m_rtCollision.w, pRootCtl->m_rtCollision.h }, text.c_str());
        pRootCtl->AddChild(pText);
        return pRootCtl;
    }

    TSlider* CreateSlider(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt)
    {
        TSlider* pRootCtl = new TSlider(pSprite);
        pRootCtl->Create(pd3dDevice, pImmediateContext);
        pRootCtl->SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });

        TWidget* pSliderButton = new TButton(I_Sprite.GetPtr(L"DXSliderButton"));
        pSliderButton->Create(pd3dDevice, pImmediateContext);
        pSliderButton->SetAttribute({ tRt.x1,  tRt.y1, pRootCtl->m_fSliderDragBtnWidth, pRootCtl->m_fSliderDragBtnHeight });
        pRootCtl->AddChild(pSliderButton);

        return pRootCtl;
    }
    TBASISUI_API TProgressBar* CreateProgressBar(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt)
    {
        TProgressBar* pRootCtl = new TProgressBar(pSprite);
        pRootCtl->Create(pd3dDevice, pImmediateContext);
        pRootCtl->SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });

        TWidget* pSliderButton = new TButton(I_Sprite.GetPtr(L"DXSliderButton"));
        pSliderButton->Create(pd3dDevice, pImmediateContext);
        pSliderButton->SetAttribute({ tRt.x1,  tRt.y1, tRt.w, tRt.h });
        pRootCtl->AddChild(pSliderButton);

        return pRootCtl;
    }

    TText* CreateText(TBasisUI* pBasisUI, D2D1_RECT_F rt, const std::wstring fontName, UINT iFontSize, CONST D2D1_COLOR_F color)
    {
        TText* pWidget = CreateText(rt, fontName, iFontSize, color);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }
    TListBox* CreateListBox(TBasisUI* pBasisUI,   TSprite* pSprite,   UINT iNumVisibleBtn, UINT iNumBtn,   TRect tRt)
    {
        TListBox* pWidget = CreateListBox(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite, iNumVisibleBtn, iNumBtn, tRt);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }

    TCheckBtn* CreateCheckButton(TBasisUI* pBasisUI,  TSprite* pSprite,   TRect tRt)
    {      
        TCheckBtn* pWidget = CreateCheckButton(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite, tRt);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }

    TButton* CreateButton(TBasisUI* pBasisUI,    TSprite* pSprite, TRect tRt, std::wstring text)
    {        
        TButton* pWidget = CreateButton(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite, tRt, text);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }

    TSlider* CreateSlider(TBasisUI* pBasisUI,     TSprite* pSprite,   TRect tRt)
    {
        TSlider* pWidget = CreateSlider(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite,  tRt);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }

    TProgressBar* CreateProgressBar(TBasisUI* pBasisUI, TSprite* pSprite, TRect tRt)
    {
        TProgressBar* pWidget = CreateProgressBar(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite, tRt);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }
};