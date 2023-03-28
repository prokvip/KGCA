#pragma once
#ifdef TBASISUI_EXPORTS
#define TBASISUI_API __declspec(dllexport)
#define TBASISUI_TEMPLATE
#else
#define TBASISUI_API __declspec(dllimport)
#define TBASISUI_TEMPLATE extern
#endif
#include "TWidget.h"
namespace TUI
{
    class TBASISUI_API TText :public TWidget
    {
    private:
        ComPtr<IDWriteTextFormat>       m_pTextFormat = nullptr;
        ComPtr<ID2D1SolidColorBrush>    m_pTextColor = nullptr;
        ComPtr<IDWriteTextLayout>       m_pTextLayout;
        ComPtr<ID2D1StrokeStyle>        m_pStrokeStyle;
        D2D1_COLOR_F            m_TextColor;
        UINT                    m_cTextLength;
        DWRITE_TEXT_METRICS     m_TextMetrics;
        D2D1_POINT_2F           m_pTextLayoutSize;
    public:
        std::wstring            m_szText;
    public:
        virtual bool SetAttribute(TRect rt, const std::vector<W_STR>& texStateList = {}) override;
        ComPtr<ID2D1StrokeStyle> CreateStrokeStyle();
        HRESULT			SetText(D2D1_POINT_2F pos, std::wstring text, D2D1::ColorF Color = { 0,0,0,1 });
        HRESULT			SetText(std::wstring text, D2D1::ColorF Color = { 0,0,0,1 });
        HRESULT			DrawTextLayout(D2D1_POINT_2F pos, D2D1::ColorF Color = { 0,0,0,1 });
        virtual bool  Create(D2D1_RECT_F rt, const std::wstring fontName = L"°íµñ", UINT iFontSize = 10, CONST D2D1_COLOR_F color = { 0,0,0,1 });
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  SetRect(TRect rt) override;
        virtual void  Move(TRect tRt)override;
    public:
        TText() { }
        TText(TSprite* pSprite) : TWidget(pSprite) {}
        virtual ~TText() {}
    };
    class TBASISUI_API TButton :public TWidget
    {
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  SetRect(TRect rt) override;
        virtual bool  SetAttribute(TRect rt, const std::vector<W_STR>& texStateList = {}) override;
        virtual void  Move(TVector2D vOffsetPos)override;
        virtual void  Move(TRect tRt)override;
    public:
        TButton() {}
        TButton(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TCheckBtn :public TWidget
    {
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        TCheckBtn() {}
        TCheckBtn(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TRadioBtn :public TWidget
    {
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        TRadioBtn() {}
        TRadioBtn(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TSlider :public TWidget
    {
    public:
        POINT       m_ptMouseDragStart;
        float       m_fSliderDragBtnWidth = 30;
        float       m_fSliderDragBtnHeight = 30;
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  Move(TRect tRt) override;
    public:
        TSlider() {}
        TSlider(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TScrollBar :public TWidget
    {
    public:
        POINT       m_ptMouseDragStart = { 0, };
        float       m_fMaxScrollBtnSizeX = 30;
        float       m_fMaxScrollBtnSizeY = 30;
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  Move(TRect tRt) override;
    public:
        TScrollBar() {}
        TScrollBar(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TProgressBar :public TWidget
    {
    public:
        POINT       m_ptMouseDragStart;
        float       m_fSliderDragBtnWidth = 30;
        float       m_fSliderDragBtnHeight = 30;
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  Move(TRect tRt) override;
    public:
        TProgressBar() {}
        TProgressBar(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TListControl : public TWidget
    {
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  SetRect(TRect rt)override;
        TListControl() {}
        TListControl(TSprite* pSprite) : TWidget(pSprite) {}
    };
    class TBASISUI_API TListBox : public TWidget
    {
    public:
        float m_fBtnWidth = 10;
        float m_fBtnHeight = 10;
        int   m_iDragBegin = 0;
        int   m_iDragEnd = 1;
        TWidget* m_pSelectBtn = nullptr;
        std::vector< TWidget*>  m_VisibleList;
        UINT    m_INumVisibleBtn = 4;
        UINT    m_INumButtonCount = 6;
        float   m_fMaxScrollBtnSizeX = 30;
        float   m_fMaxScrollBtnSizeY = 30;
    public:
        virtual bool    MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  SetRect(TRect rt)override;
        virtual TWidget* Push(TWidget* btn);
        virtual TWidget* Push(std::wstring name);
        virtual TRect GetNextRect();
        virtual void  Move(TVector2D vOffsetPos)override;
        virtual void  Move(TRect tRt)override;
    public:
        TListBox() {}
        TListBox(TSprite* pSprite) : TWidget(pSprite) {}
    };

    class TBASISUI_API TPanel : public TWidget
    {
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
    public:
        TPanel() {}
        TPanel(TSprite* pSprite) : TWidget(pSprite) {}
    };

    class TBASISUI_API TDialog : public TWidget
    {
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
    public:
        TDialog() {}
        TDialog(TSprite* pSprite) : TWidget(pSprite) {}
    };    
};

namespace TUI
{
    TBASISUI_API TText*    CreateText(D2D1_RECT_F rt, const std::wstring fontName = L"°íµñ", UINT iFontSize = 10, CONST D2D1_COLOR_F color = { 0,0,0,1 });
    TBASISUI_API TListBox* CreateListBox(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        UINT iNumVisibleBtn, UINT iNumBtn,
        TRect tRt);
    TBASISUI_API TCheckBtn* CreateCheckButton(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt);
    TBASISUI_API TButton* CreateButton(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt,
        std::wstring text = L"click");
    TBASISUI_API TSlider* CreateSlider(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt);

    TBASISUI_API TProgressBar* CreateProgressBar(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        TRect tRt);

    TBASISUI_API TText*     CreateText(TBasisUI* pBasisUI, D2D1_RECT_F rt, const std::wstring fontName = L"°íµñ", UINT iFontSize = 10, CONST D2D1_COLOR_F color = { 0,0,0,1 });
    TBASISUI_API TListBox*  CreateListBox(TBasisUI* pBasisUI,TSprite* pSprite,UINT iNumVisibleBtn, UINT iNumBtn,TRect tRt);
    TBASISUI_API TCheckBtn* CreateCheckButton(TBasisUI* pBasisUI,TSprite* pSprite,TRect tRt);
    TBASISUI_API TButton*   CreateButton(TBasisUI* pBasisUI,TSprite* pSprite,TRect tRt,std::wstring text = L"click");
    TBASISUI_API TSlider*   CreateSlider(TBasisUI* pBasisUI,TSprite* pSprite,TRect tRt);
    TBASISUI_API TProgressBar* CreateProgressBar(TBasisUI* pBasisUI, TSprite* pSprite, TRect tRt);
};