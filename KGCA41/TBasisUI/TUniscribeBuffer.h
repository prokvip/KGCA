#pragma once
#include "TWriter.h"
namespace TUI
{
    //-----------------------------------------------------------------------------
    // CUniBuffer class for the edit control
    //-----------------------------------------------------------------------------
    class TBASISUI_API CUniBuffer
    {
    private:
        WCHAR* m_pwszBuffer = nullptr;
        int         m_nBufferSize;
    public:
        IDWriteTextFormat* m_pTextFormat = nullptr;
        ComPtr<IDWriteTextLayout>       m_pTextLayout;
        ComPtr<IDWriteTypography>       m_pTypography;
        DWRITE_HIT_TEST_METRICS         m_HitClickDown = {};
        DWRITE_HIT_TEST_METRICS         m_HitClickUp = {};
        int                             m_nClickDown = 0;
        int                             m_nClickUp = 0;
        std::wstring			        m_wszText;
        UINT32					        m_cTextLength;
    public:
        bool                    SetBufferSize(int nSize);
        int                     GetTextSize();
        const WCHAR* GetBuffer();
        const WCHAR& operator[](int n) const;
        WCHAR& operator[](int n);
    public:
        HRESULT			        SetText(D2D1_POINT_2F pos, const wchar_t* text, D2D1::ColorF Color = { 0,0,0,1 });
        HRESULT                 SetTextLayout(IDWriteTextFormat* m_pTextFormat, float width, float height, const wchar_t* text = nullptr);
        HRESULT                 DrawTextLayout(D2D1_POINT_2F pos, D2D1::ColorF Color = { 0,0,0,1 });
        void                    Clear();
        bool                    InsertChar(int nIndex, WCHAR wChar);
        bool                    RemoveChar(int nIndex);
        bool                    InsertString(int nIndex, const WCHAR* pStr, int nCount = -1);
        bool                    SetText(LPCWSTR wszText);
        // Uniscribe
        bool                    CPtoX(int x, int y, BOOL& isTrailingHit, DWRITE_HIT_TEST_METRICS& hit);
        bool                    CPtoPos(int nCP, BOOL bTrail, int* pX, int* pY, DWRITE_HIT_TEST_METRICS& hit);
        bool                    XtoCP(int nX, int nY,int* pCP, int* pnTrail, BOOL& isInside, DWRITE_HIT_TEST_METRICS& hit);
        void                    GetPriorItemPos(int nCP, int* pPrior);
        void                    GetNextItemPos(int nCP, int* pPrior);
    public:
        CUniBuffer(int nInitialSize = 1);
        ~CUniBuffer();
    };
};