#include "TUniscribeBuffer.h"

namespace TUI
{
#define DXUT_MAX_EDITBOXLENGTH 0xFFFF
    int CUniBuffer::GetTextSize()
    {
        return lstrlenW(m_pwszBuffer);
    }
    const WCHAR* CUniBuffer::GetBuffer()
    {
        return m_pwszBuffer;
    }
    const WCHAR& CUniBuffer::operator[](int n) const
    {
        return m_pwszBuffer[n];
    }
    HRESULT CUniBuffer::SetTextLayout(IDWriteTextFormat* pTextFormat,float width, float height, const wchar_t* text)
    {
        HRESULT hr = S_OK;
        m_pTextFormat = pTextFormat;
        m_cTextLength = (UINT32)wcslen(text);
        m_wszText.clear();
        m_wszText = text;

        hr = I_Writer.m_pDWriteFactory->CreateTextLayout(
            m_wszText.c_str(),
            m_cTextLength,
            m_pTextFormat,
            width,
            height,
            m_pTextLayout.ReleaseAndGetAddressOf());
        return hr;
    }
    HRESULT CUniBuffer::SetText(D2D1_POINT_2F size, const wchar_t* text, D2D1::ColorF Color)
    {
        HRESULT hr = S_OK;
        m_cTextLength = (UINT32)wcslen(text);
        m_wszText.clear();
        m_wszText = text;

        hr = I_Writer.m_pDWriteFactory->CreateTextLayout(
            m_wszText.c_str(),
            m_cTextLength,
            m_pTextFormat,
            size.x,
            size.y,
            m_pTextLayout.ReleaseAndGetAddressOf());


        if (SUCCEEDED(hr))
        {
            hr = I_Writer.m_pDWriteFactory->CreateTypography(m_pTypography.GetAddressOf());
        }
        DWRITE_FONT_FEATURE fontFeature = { DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7,1 };
        if (SUCCEEDED(hr))
        {
            hr = m_pTypography->AddFontFeature(fontFeature);
        }
        DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };
        if (SUCCEEDED(hr))
        {
            hr = m_pTextLayout->SetTypography(m_pTypography.Get(), textRange);
        }
        ////https://learn.microsoft.com/ko-kr/windows/win32/directwrite/how-to-perform-hit-testing-on-a-text-layout
        //if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH)
        //{
        // DWRITE_TEXT_METRICS tm;
        //	//IDWriteTextAnalyzer* textAnalyzer;
        //	//m_pDWriteFactory->CreateTextAnalyzer(&textAnalyzer);	
        //     BOOL isTrailingHit;
        //      BOOL isInside;
        //	m_pTextLayout->GetMetrics(&tm);
        // DWRITE_HIT_TEST_METRICS hitTestMetrics;
        //	FLOAT  x = I_InputUI.m_ptPos.x;
        //	m_pTextLayout->HitTestPoint(I_InputUI.m_ptPos.x, I_InputUI.m_ptPos.y,&isTrailingHit, &isInside, &hitTestMetrics);
        //	if (isInside == TRUE)
        //	{
        //  BOOL underline = FALSE;
        //		m_pTextLayout->GetUnderline(hitTestMetrics.textPosition, &underline);
        //		//DWRITE_TEXT_RANGE textRange = { hitTestMetrics.textPosition, 1 };
        //		//m_pTextLayout->SetUnderline(!underline, textRange);
        //		m_textRange.startPosition = hitTestMetrics.textPosition;
        //		m_textRange.length = 1;
        //	}
        //}
        //if (isInside == TRUE)
        //{
        //	m_pTextLayout->SetUnderline(TRUE, m_textRange);
        //}
        return hr;
    }
    HRESULT CUniBuffer::DrawTextLayout(D2D1_POINT_2F pos, D2D1::ColorF Color)
    {
        I_Writer.m_d2dRT->BeginDraw();
        I_Writer.m_pTextColor->SetColor(Color);
        //////https://learn.microsoft.com/ko-kr/windows/win32/directwrite/how-to-perform-hit-testing-on-a-text-layout
        ////if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH)
        ////{
        // DWRITE_TEXT_METRICS tm;
        ////    m_pTextLayout->GetMetrics(&tm);
        //  //     BOOL isTrailingHit;
        //      BOOL isInside;
        ////    DWRITE_HIT_TEST_METRICS hitTestMetrics;
        ////    FLOAT  x = I_InputUI.m_ptPos.x - pos.x;
        ////    FLOAT  y = I_InputUI.m_ptPos.y - pos.y;
        ////    m_pTextLayout->HitTestPoint(x, y,&isTrailingHit, &isInside, &hitTestMetrics);
        ////    if (isInside == TRUE)
        ////    {
        ////        BOOL underline = FALSE;
        ////        m_pTextLayout->GetUnderline(hitTestMetrics.textPosition, &underline);            
        ////        m_textRange.startPosition = hitTestMetrics.textPosition;
        ////        m_textRange.length = 1;
        ////        m_wszText.erase(hitTestMetrics.textPosition, 1);
        ////    }
        ////}
        ////if (isInside == TRUE)
        ////{
        ////    FLOAT pointX;
        ////    FLOAT pointY;
        // DWRITE_TEXT_RANGE m_textRange;
        ////    HRESULT hr = m_pTextLayout->HitTestTextPosition(m_textRange.startPosition, isTrailingHit,
        ////        &pointX, &pointY, &hitTestMetrics);

        ////    UINT32 maxHitTestMetricsCount = 255;
        ////    UINT32 actualHitTestMetricsCount;
        // DWRITE_TEXT_RANGE m_textRange;
        ////    hr = m_pTextLayout->HitTestTextRange(m_textRange.startPosition,
        ////        m_wszText.size(),
        ////        0,
        ////        0,
        ////        &hitTestMetrics,
        ////        maxHitTestMetricsCount,
        ////        &actualHitTestMetricsCount);

        ////    //m_wszText.erase(std::remove(m_wszText.begin(), m_wszText.end(), L'.'), m_wszText.end());			
        ////    //auto data = m_wszText.at(hitTestMetrics.textPosition);
        ////    //m_wszText.erase(m_wszText.at(hitTestMetrics.textPosition));
        ////    m_pTextLayout->SetUnderline(TRUE, m_textRange);
        ////}
        I_Writer.m_d2dRT->DrawTextLayout(pos, m_pTextLayout.Get(), I_Writer.m_pTextColor.Get());
        I_Writer.m_d2dRT->EndDraw();
        return S_OK;
    }
    //--------------------------------------------------------------------------------------
    bool CUniBuffer::SetBufferSize(int nNewSize)
    {
        // If the current size is already the maximum allowed,
        // we can't possibly allocate more.
        if (m_nBufferSize == DXUT_MAX_EDITBOXLENGTH)
            return false;

        int nAllocateSize = (nNewSize == -1 || nNewSize < m_nBufferSize * 2) ? (m_nBufferSize ? m_nBufferSize *
            2 : 256) : nNewSize * 2;

        // Cap the buffer size at the maximum allowed.
        if (nAllocateSize > DXUT_MAX_EDITBOXLENGTH)
            nAllocateSize = DXUT_MAX_EDITBOXLENGTH;

        WCHAR* pTempBuffer = new WCHAR[nAllocateSize];
        if (!pTempBuffer)
            return false;

        ZeroMemory(pTempBuffer, sizeof(WCHAR) * nAllocateSize);

        if (m_pwszBuffer)
        {
            CopyMemory(pTempBuffer, m_pwszBuffer, m_nBufferSize * sizeof(WCHAR));
            delete[] m_pwszBuffer;
        }

        m_pwszBuffer = pTempBuffer;
        m_nBufferSize = nAllocateSize;
        return true;
    }

    //--------------------------------------------------------------------------------------
    CUniBuffer::CUniBuffer(int nInitialSize)
    {
        m_nBufferSize = 0;
        m_pwszBuffer = NULL;
        if (nInitialSize > 0)
            SetBufferSize(nInitialSize);
    }


    //--------------------------------------------------------------------------------------
    CUniBuffer::~CUniBuffer()
    {
        delete[] m_pwszBuffer;
    }


    //--------------------------------------------------------------------------------------
    WCHAR& CUniBuffer::operator[](int n)  // No param checking
    {
        return m_pwszBuffer[n];
    }


    //--------------------------------------------------------------------------------------
    void CUniBuffer::Clear()
    {
        *m_pwszBuffer = L'\0';
    }


    //--------------------------------------------------------------------------------------
    // Inserts the char at specified index.
    // If nIndex == -1, insert to the end.
    //--------------------------------------------------------------------------------------
    bool CUniBuffer::InsertChar(int nIndex, WCHAR wChar)
    {
        assert(nIndex >= 0);

        if (nIndex < 0 || nIndex > lstrlenW(m_pwszBuffer))
            return false;  // invalid index

        // Check for maximum length allowed
        if (GetTextSize() + 1 >= DXUT_MAX_EDITBOXLENGTH)
            return false;

        if (lstrlenW(m_pwszBuffer) + 1 >= m_nBufferSize)
        {
            if (!SetBufferSize(-1))
                return false;  // out of memory
        }

        assert(m_nBufferSize >= 2);

        // Shift the characters after the index, start by copying the null terminator
        WCHAR* dest = m_pwszBuffer + lstrlenW(m_pwszBuffer) + 1;
        WCHAR* stop = m_pwszBuffer + nIndex;
        WCHAR* src = dest - 1;

        while (dest > stop)
        {
            *dest-- = *src--;
        }

        // Set new character
        m_pwszBuffer[nIndex] = wChar;

        return true;
    }


    //--------------------------------------------------------------------------------------
    // Removes the char at specified index.
    // If nIndex == -1, remove the last char.
    //--------------------------------------------------------------------------------------
    bool CUniBuffer::RemoveChar(int nIndex)
    {
        if (!lstrlenW(m_pwszBuffer) || nIndex < 0 || nIndex >= lstrlenW(m_pwszBuffer))
            return false;  // Invalid index

        MoveMemory(m_pwszBuffer + nIndex, m_pwszBuffer + nIndex + 1, sizeof(WCHAR) *
            (lstrlenW(m_pwszBuffer) - nIndex));
        return true;
    }


    //--------------------------------------------------------------------------------------
    // Inserts the first nCount characters of the string pStr at specified index.
    // If nCount == -1, the entire string is inserted.
    // If nIndex == -1, insert to the end.
    //--------------------------------------------------------------------------------------
    bool CUniBuffer::InsertString(int nIndex, const WCHAR* pStr, int nCount)
    {
        assert(nIndex >= 0);
        if (nIndex < 0)
            return false;

        if (nIndex > lstrlenW(m_pwszBuffer))
            return false;  // invalid index

        if (-1 == nCount)
            nCount = lstrlenW(pStr);

        // Check for maximum length allowed
        if (GetTextSize() + nCount >= DXUT_MAX_EDITBOXLENGTH)
            return false;

        if (lstrlenW(m_pwszBuffer) + nCount >= m_nBufferSize)
        {
            if (!SetBufferSize(lstrlenW(m_pwszBuffer) + nCount + 1))
                return false;  // out of memory
        }

        MoveMemory(m_pwszBuffer + nIndex + nCount, m_pwszBuffer + nIndex, sizeof(WCHAR) *
            (lstrlenW(m_pwszBuffer) - nIndex + 1));
        CopyMemory(m_pwszBuffer + nIndex, pStr, nCount * sizeof(WCHAR));
        return true;
    }


    //--------------------------------------------------------------------------------------
    bool CUniBuffer::SetText(LPCWSTR wszText)
    {
        assert(wszText != NULL);

        int nRequired = int(wcslen(wszText) + 1);

        // Check for maximum length allowed
        if (nRequired >= DXUT_MAX_EDITBOXLENGTH)
            return false;

        while (m_nBufferSize < nRequired)
            if (!SetBufferSize(-1))
                break;
        // Check again in case out of memory occurred inside while loop.
        if (m_nBufferSize >= nRequired)
        {
            wcscpy_s(m_pwszBuffer, m_nBufferSize, wszText);
            return true;
        }
        else
            return false;
    }


    //--------------------------------------------------------------------------------------
    bool CUniBuffer::CPtoX(int x, int y, BOOL& isTrailingHit, DWRITE_HIT_TEST_METRICS& hit)
    {
        if (m_pTextLayout == nullptr) return false;
        HRESULT hr = S_OK;
        BOOL isInside;
        m_pTextLayout->HitTestPoint((FLOAT)x, (FLOAT)y, &isTrailingHit, &isInside, &hit);
        if (isInside == TRUE)
        {
            return true;
        }
        return false;
    }
    bool CUniBuffer::CPtoPos(int nCP, BOOL bTrail, int* pX, int* pY, DWRITE_HIT_TEST_METRICS& hit)
    {
        if (pX != nullptr)
        {
            *pX = 0;  // Default
        }
        if (m_pTextLayout == nullptr) return false;

        HRESULT hr = S_OK;
        FLOAT pointX;
        FLOAT pointY;
        hr = m_pTextLayout->HitTestTextPosition(nCP, TRUE, &pointX, &pointY, &hit);
        if (SUCCEEDED(hr))
        {
            if (pX != nullptr)    *pX = (int)pointX;
            if (pY != nullptr)    *pY = (int)pointY;
            return true;
        }
        return false;
    }


    //--------------------------------------------------------------------------------------
    bool CUniBuffer::XtoCP(int nX, int nY, int* pCP, int* pnTrail, BOOL& isInside, DWRITE_HIT_TEST_METRICS& hit)
    {
        assert(pCP && pnTrail);
        *pCP = 0; *pnTrail = FALSE;  // Default

        if (m_pTextLayout == nullptr) return false;

        //BOOL isInside;
        HRESULT hr = m_pTextLayout->HitTestPoint((FLOAT)nX, (FLOAT)nY, pnTrail, &isInside, &hit);
        if (SUCCEEDED(hr)/* && isInside == TRUE*/)
        {
            *pCP = hit.textPosition;
            return true;
        }
     /*   if( isInside == TRUE)
        {
            *pCP = hit.textPosition;
            return true;
        }*/
        return false;
    }


    //--------------------------------------------------------------------------------------
    void CUniBuffer::GetPriorItemPos(int nCP, int* pPrior)
    {
        *pPrior = nCP;  // Default is the char itself  
        *pPrior = 0;
    }


    //--------------------------------------------------------------------------------------
    void CUniBuffer::GetNextItemPos(int nCP, int* pPrior)
    {
        *pPrior = nCP;  // Default is the char itself   
    }

};