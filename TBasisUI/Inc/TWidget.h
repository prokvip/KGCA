#pragma once
#ifdef TBASISUI_EXPORTS
#define TBASISUI_API __declspec(dllexport)
#define TBASISUI_TEMPLATE
#else
#define TBASISUI_API __declspec(dllimport)
#define TBASISUI_TEMPLATE extern
#endif
#include "TSpriteManager.h"
#include "TWriter.h"
#include "TInputUI.h"
//https://www.vectorstock.com/
namespace TUI
{
    //--------------------------------------------------------------------------------------
    // Enums for pre-defined control types
    //--------------------------------------------------------------------------------------
    enum TCONTROL_TYPE
    {
        T_CONTROL_BUTTON,
        T_CONTROL_STATIC,
        T_CONTROL_CHECKBOX,
        T_CONTROL_RADIOBUTTON,
        T_CONTROL_COMBOBOX,
        T_CONTROL_SLIDER,
        T_CONTROL_EDITBOX,
        T_CONTROL_IMEEDITBOX,
        T_CONTROL_LISTBOX,
        T_CONTROL_SCROLLBAR,
        T_CONTROL_PROGRESSBAR,
        T_DIGALOG,
    };

    enum T_CONTROL_STATE
    {
        T_STATE_NORMAL = 0,
        T_STATE_MOUSEOVER,// HOVER
        T_STATE_PRESSED,  // PUSH
        T_STATE_DISABLED,
        T_STATE_HIDDEN,
        T_STATE_FOCUS,
        T_STATE_HOLD,       // HOLD
        T_STATE_SELECT,   // PUSH+UP
        T_STATE_MAXSTATE,
    };
    class TWidget;
    typedef std::multiset<std::pair<int, TWidget*>> TPriorityMap;

    class TBASISUI_API TWidget :public TObject2D
    {
    public:
        static  int     g_iNumCounter;
        static          TWidget* m_pSelectEditWidget;
    protected:
        int             m_ID;
        TCONTROL_TYPE   m_Type;
        UINT            m_nHotkey;
        bool            m_bEnabled;
        RECT            m_rcBoundingBox;
        int             m_iDrawID = 0;
        int             m_iPriority;
        TPriorityMap*	m_pPriorityQueue=nullptr;
        TPriorityMap::iterator m_Iterator;   
    public:
        std::vector<TWidget*>  m_pPostDrawWidget;
        std::vector<TWidget*>  m_pSavePostDrawWidget;
        TWidget*        m_pParentWidget = nullptr;
        TVector2D       m_vOffset;
        float           m_fPercentage = 0.0f;
        bool            m_bMouseDrag = false;
        bool            m_bKeyDrag = false;
        bool            m_bMovement = true;
        T_CONTROL_STATE			m_CurrentState;
        std::vector<TTexture*>  m_pStateList;
        TTexture* m_pCurrentTex = nullptr;
        TSprite* m_pSprite = nullptr;
        std::vector<TWidget*> m_rtDrawList;
        std::vector<TWidget*> m_pChildList;
        TRect		m_tRect = { 0,0,0,0 };
        bool		m_bToggle = false;
        bool        m_bVisible;
        bool        m_bMouseOver;
        bool        m_bHasFocus;
        bool        m_bIsDefault;
      public:
        void        Add(TPriorityMap& queue);
        void        Remove();
        int         GetPriority() const;
        void        SetPriority(int priority);
        int         GetDrawID() { return m_iDrawID; }
        void        SetDrawID(int id) { m_iDrawID = id; }
        //--------------------------------------------------------------------------------------
        virtual bool    MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual bool    HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual bool    HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
        virtual void    UpdateRects();
        virtual void    Refresh();
        virtual bool    CanHaveFocus();
        virtual void    OnFocusIn();
        virtual void    OnFocusOut();
        virtual void    OnMouseEnter();
        virtual void    OnMouseLeave();
        virtual void    OnHotkey();
        virtual BOOL    ContainsPoint(POINT pt);
        virtual void    SetEnabled(bool bEnabled);
        virtual bool    GetEnabled();
        virtual void    SetVisible(bool bVisible);
        virtual bool    GetVisible();
        virtual UINT    GetType() const;
        virtual int     GetID() const;
        virtual void    SetID(int ID);
        virtual void    SetHotkey(UINT nHotkey);
        virtual UINT    GetHotkey();
        virtual void    SetTextColor(DWORD Color) {};
        virtual bool    Frame();
        virtual bool    Release();
        virtual void    FadeInOut(float fAlpha);
        virtual void    AddChild(TWidget* pUI);
        virtual bool    SetTextueState(const std::vector<W_STR>& texStateList);
        virtual void    Move(TVector2D vOffsetPos);
        virtual void    Move(TRect tRt);
        virtual bool    SetAttribute(TRect rt, const std::vector<W_STR>& texStateList = {});
        virtual bool    SetAttribute(TVector2D vPos, const std::vector<W_STR>& texStateList = std::vector<W_STR>());
        virtual void    SetSprite(TSprite* pSprite);
        virtual bool    SetDrawList(float fScaleX0, float fScaleX1, float fScaleY0, float fScaleY1, float fScaleU0, float fScaleU1, float fScaleV0, float fScaleV1);
        virtual bool    SetDrawList(float fScaleX0, float fScaleX1, float fScaleY0, float fScaleY1, TSprite* pSprite);
        virtual bool    Push(TWidget* btn);
        virtual bool    Push(std::wstring name);
        virtual TRect   GetNextRect();
        virtual TVector4D GetSpriteOffsetUV();
        virtual void    SetPostDrawList();
        virtual void    PostDrawList();
    public:
        TWidget();
        TWidget(TSprite* pSprite);
        virtual ~TWidget() {}
    };

};