#pragma once
#ifdef TBASISUI_EXPORTS
#define TBASISUI_API __declspec(dllexport)
#define TBASISUI_TEMPLATE
#else
#define TBASISUI_API __declspec(dllimport)
#define TBASISUI_TEMPLATE extern
#endif
#include "TUniscribeBuffer.h"
#include "TWidgetControl.h"
namespace TUI
{
    class TBASISUI_API TEditBox : public TWidget
    {
    protected:
        int     m_nBorder;      // Border of the window
        int     m_nSpacing;     // Spacing between the text and the edge of border
        RECT    m_rcText;       // Bounding rectangle for the text
        RECT    m_rcRender[9];  // Convenient rectangles for rendering elements
        double  m_dfBlink;      // Caret blink time in milliseconds
        double  m_dfLastBlink;  // Last timestamp of caret blink
        bool    m_bCaretOn;     // Flag to indicate whether caret is currently visible
        int     m_nCaret;       // Caret position, in characters
        bool    m_bInsertMode;  // If true, control is in insert mode. Else, overwrite mode.
        int     m_nSelStart;    // Starting position of the selection. The caret marks the end.
        int     m_nFirstVisible;// First visible character in the edit control
        DWORD   m_TextColor;    // Text color
        DWORD   m_SelTextColor; // Selected text color
        DWORD   m_SelBkColor;   // Selected background color
        DWORD   m_CaretColor;   // Caret color
        static bool s_bHideCaret;   // If true, we don't render the caret.
    public:
        CUniBuffer m_Buffer;
    public:
        virtual void    SetRect(TRect rt) override;
        virtual bool    Render() override;
        virtual bool    HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual bool    HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
        virtual bool    MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void    UpdateRects();
        virtual bool    CanHaveFocus();
        virtual void    OnFocusIn();
        void            SetText(LPCWSTR wszText, bool bSelected = false);
        LPCWSTR         GetText();
        int             GetTextLength();
        HRESULT         GetTextCopy(__out_ecount(bufferCount) LPWSTR strDest, UINT bufferCount);
        void            ClearText();
        virtual void    SetTextColor(DWORD Color);
        void            SetSelectedTextColor(DWORD Color);
        void            SetSelectedBackColor(DWORD Color);
        void            SetCaretColor(DWORD Color);
        void            SetBorderWidth(int nBorder);
        void            SetSpacing(int nSpacing);
        void            ParseFloatArray(float* pNumbers, int nCount);
        void            SetTextFloatArray(const float* pNumbers, int nCount);
    public:
        void            DragKeyBegin();
        void            DragKeyEnd();
        void            Move(TRect tRt);
    protected:
        void            PlaceCaret(int nCP);
        void            DeleteSelectionText();
        void            ResetCaretBlink();
        void            CopyToClipboard();
        void            PasteFromClipboard();
    public:
        TEditBox();
        TEditBox(TSprite* pSprite) : TWidget(pSprite) {}
        virtual         ~TEditBox();
    };

    class TBASISUI_API TComboBox : public TEditBox
    {
        bool        m_bVisibleListBtn = false;
    public:
        float m_fBtnHeight = 10;
        int   m_iDragBegin = 0;
        int   m_iDragEnd = 1;
        std::vector< TWidget*>  m_VisibleList;
    public:
        virtual bool  Init() override;
        virtual bool  Frame() override;
        virtual bool  Render() override;
        virtual bool  Release() override;
        virtual void  SetRect(TRect rt)override;
        virtual TWidget* Push(TWidget* btn)override;
        virtual TWidget* Push(std::wstring name)override;
        virtual TRect GetNextRect()override;
        virtual void  Move(TVector2D vOffsetPos)override;
        virtual void  Move(TRect tRt)override;
    public:
        TComboBox() {}
        TComboBox(TSprite* pSprite) : TEditBox(pSprite) {}
    };
   
}; 

namespace TUI
{
    TBASISUI_API TEditBox* CreateEditBox(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        IDWriteTextFormat* pTextFormat,
        TRect tRt);

    TBASISUI_API TComboBox* CreateComboBox(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        IDWriteTextFormat* pTextFormat,
        TRect tRt, UINT iNumVisibleBtn = 4, UINT iNumBtn = 5);


    TBASISUI_API TEditBox* CreateEditBox(TBasisUI* pBasisUI,TSprite* pSprite, IDWriteTextFormat* pTextFormat, TRect tRt);
    TBASISUI_API TComboBox* CreateComboBox(TBasisUI* pBasisUI,TSprite* pSprite,IDWriteTextFormat* pTextFormat, TRect tRt, UINT iNumVisibleBtn=4, UINT iNumBtn=5);
};