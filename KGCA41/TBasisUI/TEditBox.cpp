#include "TEditBox.h"
#include "TDxStateUI.h"
#include "TBasisUI.h"

namespace TUI
{
    // maps unsigned 8 bits/channel to DWORD
#define D3DCOLOR_ARGB(a,r,g,b) \
    ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
#define D3DCOLOR_XRGB(r,g,b)   D3DCOLOR_ARGB(0xff,r,g,b)

#define D3DCOLOR_XYUV(y,u,v)   D3DCOLOR_ARGB(0xff,y,u,v)
#define D3DCOLOR_AYUV(a,y,u,v) D3DCOLOR_ARGB(a,y,u,v)

// maps floating point channels (0.f to 1.f range) to DWORD
#define D3DCOLOR_COLORVALUE(r,g,b,a) \
    D3DCOLOR_RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))

#define IN_FLOAT_CHARSET( c ) \
    ( (c) == L'-' || (c) == L'.' || ( (c) >= L'0' && (c) <= L'9' ) )

    bool    TEditBox::s_bHideCaret;   // If true, we don't render the caret.

    inline int RectWidth(RECT& rc)
    {
        return ((rc).right - (rc).left);
    }
    inline int RectHeight(RECT& rc)
    {
        return ((rc).bottom - (rc).top);
    }

#define EDITBOX_SCROLLEXTENT 4
    //--------------------------------------------------------------------------------------
    TEditBox::TEditBox()
    {
        m_Type = T_CONTROL_EDITBOX;
        m_nBorder = 5;  // Default border width
        m_nSpacing = 4;  // Default spacing
        m_bCaretOn = true;
        m_dfBlink = GetCaretBlinkTime() * 0.001f;
        m_dfLastBlink = g_fGameTimer;// DXUTGetGlobalTimer()->GetAbsoluteTime();
        s_bHideCaret = false;
        m_nFirstVisible = 0;
        m_TextColor = D3DCOLOR_ARGB(255, 16, 16, 16);
        m_SelTextColor = D3DCOLOR_ARGB(255, 255, 255, 255);
        m_SelBkColor = D3DCOLOR_ARGB(255, 40, 50, 92);
        m_CaretColor = D3DCOLOR_ARGB(255, 0, 0, 0);
        m_nCaret = m_nSelStart = 0;
        m_bInsertMode = true;
        m_bMouseDrag = false;
        m_bEnabled = true;
        m_bVisible = true;
        m_bMouseOver = false;
        m_bHasFocus = false;
        m_bIsDefault = false;
         ZeroMemory(&m_rcBoundingBox, sizeof(m_rcBoundingBox));
    }
    //--------------------------------------------------------------------------------------
    TEditBox::~TEditBox()
    {
    }
    bool    TEditBox::CanHaveFocus()
    {
        return (m_bVisible && m_bEnabled);
    }
    LPCWSTR         TEditBox::GetText()
    {
        return m_Buffer.GetBuffer();
    }
    int             TEditBox::GetTextLength()
    {
        return m_Buffer.GetTextSize();
    }
    void    TEditBox::SetTextColor(DWORD Color)
    {
        m_TextColor = Color;
    }
    // Text color
    void            TEditBox::SetSelectedTextColor(DWORD Color)
    {
        m_SelTextColor = Color;
    }
    // Selected text color
    void            TEditBox::SetSelectedBackColor(DWORD Color)
    {
        m_SelBkColor = Color;
    }
    // Selected background color
    void            TEditBox::SetCaretColor(DWORD Color)
    {
        m_CaretColor = Color;
    }
    // Caret color
    void            TEditBox::SetBorderWidth(int nBorder)
    {
        m_nBorder = nBorder; UpdateRects();
    }
    // Border of the window
    void            TEditBox::SetSpacing(int nSpacing)
    {
        m_nSpacing = nSpacing; UpdateRects();
    }
    void TEditBox::PlaceCaret(int nCP)
    {
        m_nCaret = nCP;
    }
    //--------------------------------------------------------------------------------------
    void TEditBox::ClearText()
    {
        m_Buffer.Clear();
        m_nFirstVisible = 0;
        PlaceCaret(0);
        m_nSelStart = 0;
    }
    //--------------------------------------------------------------------------------------
    void TEditBox::SetText(LPCWSTR wszText, bool bSelected)
    {
        assert(wszText != NULL);

        m_Buffer.SetText(wszText);
        m_nFirstVisible = 0;
        // Move the caret to the end of the text
        PlaceCaret(m_Buffer.GetTextSize());
        m_nSelStart = bSelected ? 0 : m_nCaret;
    }
    //--------------------------------------------------------------------------------------
    HRESULT TEditBox::GetTextCopy(__out_ecount(bufferCount) LPWSTR strDest,UINT bufferCount)
    {
        assert(strDest);

        wcscpy_s(strDest, bufferCount, m_Buffer.GetBuffer());

        return S_OK;
    }
    //--------------------------------------------------------------------------------------
    void TEditBox::DeleteSelectionText()
    {
        int nFirst = __min(m_Buffer.m_HitClickDown.textPosition, m_Buffer.m_HitClickUp.textPosition);
        int nLast = __max(m_Buffer.m_HitClickDown.textPosition, m_Buffer.m_HitClickUp.textPosition);
        // Update caret and selection
        PlaceCaret(nFirst);
        m_nSelStart = m_nCaret;
        // Remove the characters
        for (int i = nFirst; i < nLast; ++i)
            m_Buffer.RemoveChar(nFirst);
    }
    //--------------------------------------------------------------------------------------
    void TEditBox::UpdateRects()
    {
        TWidget::UpdateRects();

        // Update the text rectangle
        m_rcText = m_rcBoundingBox;
        // First inflate by m_nBorder to compute render rects
        ::InflateRect(&m_rcText, -m_nBorder, -m_nBorder);

        // Update the render rectangles
        m_rcRender[0] = m_rcText;
        ::SetRect(&m_rcRender[1], m_rcBoundingBox.left, m_rcBoundingBox.top, m_rcText.left, m_rcText.top);
        ::SetRect(&m_rcRender[2], m_rcText.left, m_rcBoundingBox.top, m_rcText.right, m_rcText.top);
        ::SetRect(&m_rcRender[3], m_rcText.right, m_rcBoundingBox.top, m_rcBoundingBox.right, m_rcText.top);
        ::SetRect(&m_rcRender[4], m_rcBoundingBox.left, m_rcText.top, m_rcText.left, m_rcText.bottom);
        ::SetRect(&m_rcRender[5], m_rcText.right, m_rcText.top, m_rcBoundingBox.right, m_rcText.bottom);
        ::SetRect(&m_rcRender[6], m_rcBoundingBox.left, m_rcText.bottom, m_rcText.left, m_rcBoundingBox.bottom);
        ::SetRect(&m_rcRender[7], m_rcText.left, m_rcText.bottom, m_rcText.right, m_rcBoundingBox.bottom);
        ::SetRect(&m_rcRender[8], m_rcText.right, m_rcText.bottom, m_rcBoundingBox.right, m_rcBoundingBox.bottom);

        // Inflate further by m_nSpacing
        ::InflateRect(&m_rcText, -m_nSpacing, -m_nSpacing);
    }
    void TEditBox::CopyToClipboard()
    {
        // Copy the selection text to the clipboard
        if (m_nCaret != m_nSelStart && OpenClipboard(NULL))
        {
            EmptyClipboard();

            HGLOBAL hBlock = GlobalAlloc(GMEM_MOVEABLE, sizeof(WCHAR) * (m_Buffer.GetTextSize() + 1));
            if (hBlock)
            {
                WCHAR* pwszText = (WCHAR*)GlobalLock(hBlock);
                if (pwszText)
                {
                    int nFirst = __min(m_nCaret, m_nSelStart);
                    int nLast = __max(m_nCaret, m_nSelStart);
                    if (nLast - nFirst > 0)
                        CopyMemory(pwszText, m_Buffer.GetBuffer() + nFirst, (nLast - nFirst) * sizeof(WCHAR));
                    pwszText[nLast - nFirst] = L'\0';  // Terminate it
                    GlobalUnlock(hBlock);
                }
                SetClipboardData(CF_UNICODETEXT, hBlock);
            }
            CloseClipboard();
            // We must not free the object until CloseClipboard is called.
            if (hBlock)
                GlobalFree(hBlock);
        }
    }
    void TEditBox::PasteFromClipboard()
    {
        DeleteSelectionText();

        if (OpenClipboard(NULL))
        {
            HANDLE handle = GetClipboardData(CF_UNICODETEXT);
            if (handle)
            {
                // Convert the ANSI string to Unicode, then
                // insert to our buffer.
                WCHAR* pwszText = (WCHAR*)GlobalLock(handle);
                if (pwszText)
                {
                    // Copy all characters up to null.
                    if (m_Buffer.InsertString(m_nCaret, pwszText))
                        PlaceCaret(m_nCaret + lstrlenW(pwszText));
                    m_nSelStart = m_nCaret;
                    GlobalUnlock(handle);
                }
            }
            CloseClipboard();
        }
    }
    //--------------------------------------------------------------------------------------
    bool TEditBox::Render()
    {
        if (m_bVisible == false)
            return true;

        m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pImmediateContext->RSSetState(TUI::TDxStateUI::g_pUI_SolidRS);
        m_pImmediateContext->OMSetBlendState(TUI::TDxStateUI::g_pUI_AlphaBlend, 0, -1);

        for (int iSub = 0; iSub < m_rtDrawList.size(); iSub++)
        {
            m_rtDrawList[iSub]->Render();
        }

        //
        // Render the selection rectangle
        //
        if (m_Buffer.m_nClickDown != m_Buffer.m_nClickUp)
        {
            if (m_Buffer.m_nClickDown < m_Buffer.m_nClickUp) // 왼쪽에서 오른쪽 드래그
            {
                D2D1_RECT_F rt = { m_Buffer.m_HitClickDown.left + m_rcText.left,
                                       m_Buffer.m_HitClickDown.top + m_rcText.top,
                                       m_Buffer.m_HitClickUp.left + m_Buffer.m_HitClickUp.width + m_rcText.left,
                                       m_Buffer.m_HitClickUp.top + m_Buffer.m_HitClickUp.height + m_rcText.top };
                I_Writer.DrawRectangle(rt);
            }
            else// 오른쪽에서 왼쪽 드래그
            {
                D2D1_RECT_F rt = { m_Buffer.m_HitClickUp.left + m_rcText.left,
                                      m_Buffer.m_HitClickUp.top + m_rcText.top,
                                      m_Buffer.m_HitClickDown.left + m_Buffer.m_HitClickDown.width + m_rcText.left,
                                      m_Buffer.m_HitClickDown.top + m_Buffer.m_HitClickDown.height + m_rcText.top };
                I_Writer.DrawRectangle(rt);
            }
        }
        //
        // Render the text
        //
        D2D1_RECT_F rt = { (float)m_rcText.left, (float)m_rcText.top,(float)m_rcText.right, (float)m_rcText.bottom };
        m_Buffer.SetText({ (float)g_rtClient.right, (float)g_rtClient.bottom }, m_Buffer.GetBuffer() + m_nFirstVisible);
        m_Buffer.DrawTextLayout({ (float)m_rcText.left,  (float)m_rcText.top });
        //
        // Blink the caret
        //
        if (g_fGameTimer - m_dfLastBlink >= m_dfBlink)
        {
            m_bCaretOn = !m_bCaretOn;
            m_dfLastBlink = g_fGameTimer;
        }
        //
        // Render the caret if this control has the focus
        //
        if (m_bHasFocus && m_bCaretOn && !s_bHideCaret)
        {
            int nX1st, nY1st;
            DWRITE_HIT_TEST_METRICS hit;
            if (m_Buffer.CPtoPos(m_nCaret, TRUE, &nX1st, &nY1st, hit))
            {
                D2D1_RECT_F rt = { hit.left + m_rcText.left,
                                    hit.top + m_rcText.top,
                                    hit.width + m_rcText.right,
                                    hit.height + m_rcText.bottom };

                I_Writer.Draw(rt, L"_");
            }
        }

        PostDrawList();
        return true;
    }
    void TEditBox::ParseFloatArray(float* pNumbers, int nCount)
    {
        int nWritten = 0;  // Number of floats written
        const WCHAR* pToken, * pEnd;
        WCHAR wszToken[60];

        pToken = m_Buffer.GetBuffer();
        while (nWritten < nCount && *pToken != L'\0')
        {
            // Skip leading spaces
            while (*pToken == L' ')
                ++pToken;

            if (*pToken == L'\0')
                break;

            // Locate the end of number
            pEnd = pToken;
            while (IN_FLOAT_CHARSET(*pEnd))
                ++pEnd;

            // Copy the token to our buffer
            int nTokenLen = __min(sizeof(wszToken) / sizeof(wszToken[0]) - 1, int(pEnd - pToken));
            wcscpy_s(wszToken, nTokenLen, pToken);
            *pNumbers = (float)wcstod(wszToken, NULL);
            ++nWritten;
            ++pNumbers;
            pToken = pEnd;
        }
    }
    void TEditBox::SetTextFloatArray(const float* pNumbers, int nCount)
    {
        WCHAR wszBuffer[512] =
        {
            0
        };
        WCHAR wszTmp[64];

        if (pNumbers == NULL)
            return;

        for (int i = 0; i < nCount; ++i)
        {
            swprintf_s(wszTmp, 64, L"%.4f ", pNumbers[i]);
            wcscat_s(wszBuffer, 512, wszTmp);
        }

        // Don't want the last space
        if (nCount > 0 && wcslen(wszBuffer) > 0)
            wszBuffer[wcslen(wszBuffer) - 1] = 0;

        SetText(wszBuffer);
    }
    //--------------------------------------------------------------------------------------
    void TEditBox::OnFocusIn()
    {
        TWidget::OnFocusIn();
        ResetCaretBlink();
    }
    void TEditBox::ResetCaretBlink()
    {
        m_bCaretOn = true;
        m_dfLastBlink = g_fGameTimer;
    }
    void TEditBox::SetRect(TRect rt)
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
    //--------------------------------------------------------------------------------------
    bool TEditBox::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (!m_bEnabled || !m_bVisible)
            return false;
       
        bool bHandled = false;
        switch (uMsg)
        {
            // Keyboard messages
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            if (HandleKeyboard(uMsg, wParam, lParam))
            {
                return true;
            }
        }
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
            bHandled = HandleMouse(uMsg, mousePoint, wParam, lParam);
            if (bHandled)
                return true;
            break;
        }

        case WM_CAPTURECHANGED:
        {
            // The application has lost mouse capture.
            // The dialog object may not have received
            // a WM_MOUSEUP when capture changed. Reset
            // m_bDrag so that the dialog does not mistakenly
            // think the mouse button is still held down.
            if ((HWND)lParam != g_hWnd)
                m_bMouseDrag = false;
        }break;

        case WM_CHAR:
        {
            if (m_bHasFocus == false||
                m_pSelectEditWidget!=this) break;
            switch ((WCHAR)wParam)
            {
                // Backspace
            case VK_BACK:
            {
                // If there's a selection, treat this
                // like a delete key.
                if (m_nCaret != m_nSelStart)
                {
                    DeleteSelectionText();
                }
                else if (m_nCaret > 0)
                {
                    // Move the caret, then delete the char.
                    PlaceCaret(m_nCaret - 1);
                    m_nSelStart = m_nCaret;
                    m_Buffer.RemoveChar(m_nCaret);
                }
                ResetCaretBlink();
                break;
            }

            case 24:        // Ctrl-X Cut
            case VK_CANCEL: // Ctrl-C Copy
            {
                CopyToClipboard();

                // If the key is Ctrl-X, delete the selection too.
                if ((WCHAR)wParam == 24)
                {
                    DeleteSelectionText();
                }

                break;
            }

            // Ctrl-V Paste
            case 22:
            {
                PasteFromClipboard();
                break;
            }

            // Ctrl-A Select All
            case 1:
                if (m_nSelStart == m_nCaret)
                {
                    m_nSelStart = 0;
                    PlaceCaret(m_Buffer.GetTextSize());
                }
                break;

            case VK_RETURN:
                // Invoke the callback when the user presses Enter.
                //m_pDialog->SendEvent(EVENT_EDITBOX_STRING, true, this);
                break;

                // Junk characters we don't want in the string
            case 26:  // Ctrl Z
            case 2:   // Ctrl B
            case 14:  // Ctrl N
            case 19:  // Ctrl S
            case 4:   // Ctrl D
            case 6:   // Ctrl F
            case 7:   // Ctrl G
            case 10:  // Ctrl J
            case 11:  // Ctrl K
            case 12:  // Ctrl L
            case 17:  // Ctrl Q
            case 23:  // Ctrl W
            case 5:   // Ctrl E
            case 18:  // Ctrl R
            case 20:  // Ctrl T
            case 25:  // Ctrl Y
            case 21:  // Ctrl U
            case 9:   // Ctrl I
            case 15:  // Ctrl O
            case 16:  // Ctrl P
            case 27:  // Ctrl [
            case 29:  // Ctrl ]
            case 28:  // Ctrl \ 
                break;

            default:
            {
                // If there's a selection and the user
                // starts to type, the selection should
                // be deleted.
                if (m_nCaret != m_nSelStart)
                    DeleteSelectionText();

                // If we are in overwrite mode and there is already
                // a char at the caret's position, simply replace it.
                // Otherwise, we insert the char as normal.
                if (!m_bInsertMode && m_nCaret < m_Buffer.GetTextSize())
                {
                    m_Buffer[m_nCaret] = (WCHAR)wParam;
                    PlaceCaret(m_nCaret + 1);
                    m_nSelStart = m_nCaret;
                }
                else
                {
                    // Insert the char
                    if (m_Buffer.InsertChar(m_nCaret, (WCHAR)wParam))
                    {
                        PlaceCaret(m_nCaret + 1);
                        m_nSelStart = m_nCaret;
                    }
                }
                ResetCaretBlink();
            }
            }
            return true;
        }
        }
        return false;
    }
    //--------------------------------------------------------------------------------------
    bool TEditBox::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (!m_bEnabled || !m_bVisible || !m_bHasFocus)
            return false;

        bool bHandled = false;

        switch (uMsg)
        {
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_TAB:
                // We don't process Tab in case keyboard input is enabled and the user
                // wishes to Tab to other controls.
                break;

            case VK_HOME:
                PlaceCaret(0);
                if (GetKeyState(VK_SHIFT) >= 0)
                    m_nSelStart = m_nCaret;
                ResetCaretBlink();
                bHandled = true;
                break;

            case VK_END:
                PlaceCaret(m_Buffer.GetTextSize());
                if (GetKeyState(VK_SHIFT) >= 0)
                    m_nSelStart = m_nCaret;
                ResetCaretBlink();
                bHandled = true;
                break;

            case VK_INSERT:
                if (GetKeyState(VK_CONTROL) < 0)
                {
                    // Control Insert. Copy to clipboard
                    CopyToClipboard();
                }
                else if (GetKeyState(VK_SHIFT) < 0)
                {
                    // Shift Insert. Paste from clipboard
                    PasteFromClipboard();
                }
                else
                {
                    // Toggle caret insert mode
                    m_bInsertMode = !m_bInsertMode;
                }
                break;

            case VK_DELETE:
                if (m_Buffer.m_HitClickDown.textPosition != m_Buffer.m_HitClickUp.textPosition)
                {
                    DeleteSelectionText();
                    if (m_Buffer.CPtoPos(m_nCaret, FALSE, nullptr, nullptr, m_Buffer.m_HitClickDown))
                    {
                        m_Buffer.m_nClickUp = m_Buffer.m_nClickDown;
                        ResetCaretBlink();
                    }
                    m_bKeyDrag = false;
                }
                m_Buffer.RemoveChar(m_nCaret);
                ResetCaretBlink();
                bHandled = true;
                break;
            case VK_SHIFT:
            {
                break;
            }
            case VK_LEFT:
                DragKeyBegin();
                if (GetKeyState(VK_CONTROL) < 0)
                {
                    m_Buffer.GetPriorItemPos(m_nCaret, &m_nCaret);
                    PlaceCaret(m_nCaret);
                }
                else if (m_nCaret > 0)
                {
                    PlaceCaret(m_nCaret - 1);
                    DragKeyEnd();
                }

                ResetCaretBlink();
                bHandled = true;
                break;

            case VK_RIGHT:
                DragKeyBegin();
                if (GetKeyState(VK_CONTROL) < 0)
                {
                    m_Buffer.GetNextItemPos(m_nCaret, &m_nCaret);
                    PlaceCaret(m_nCaret);
                }
                else if (m_nCaret < m_Buffer.GetTextSize())
                {
                    PlaceCaret(m_nCaret + 1);
                    DragKeyEnd();
                }
                ResetCaretBlink();
                bHandled = true;
                break;

            case VK_UP:
            case VK_DOWN:
                // Trap up and down arrows so that the dialog
                // does not switch focus to another control.
                bHandled = true;
                break;

            default:
                bHandled = wParam != VK_ESCAPE;  // Let the application handle Esc.
            }
        }
        }
        return bHandled;
    }
    //--------------------------------------------------------------------------------------
    bool TEditBox::HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
    {
        if (!m_bEnabled || !m_bVisible)
            return false;

        switch (uMsg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        {
            if (!ContainsPoint(pt))
            {
                OnFocusOut();
                return false;
            }
            m_pSelectEditWidget = this;
            OnFocusIn();
            m_bMouseDrag = true;
            SetCapture(g_hWnd);

            int  x = pt.x - m_rcText.left;
            int  y = pt.y - m_rcText.top;
            int nCP, nTrail;
            BOOL isInside;
            if (m_Buffer.XtoCP((int)x,y, &nCP, &nTrail, isInside, m_Buffer.m_HitClickDown))
            {
                m_Buffer.m_nClickDown = nCP;
                m_Buffer.m_nClickUp = nCP;
                m_Buffer.m_HitClickUp = m_Buffer.m_HitClickDown;

                if (nTrail)
                    PlaceCaret(nCP + 1);
                else
                    PlaceCaret(nCP);
                m_nSelStart = m_nCaret;
                ResetCaretBlink();
            }
            /*else
            {
                m_Buffer.m_nClickDown = m_Buffer.m_HitClickDown.textPosition + 1;
                m_Buffer.m_nClickUp = m_Buffer.m_nClickDown;
                m_Buffer.m_HitClickUp = m_Buffer.m_HitClickDown;
                PlaceCaret(m_Buffer.m_HitClickDown.textPosition + 1);
            }*/
            return true;
        }

        case WM_LBUTTONUP:
            ReleaseCapture();
            m_bMouseDrag = false;
            break;

        case WM_MOUSEMOVE:
            if (!ContainsPoint(pt))
            {
                OnFocusOut();
            }
            OnFocusIn();
            if (m_bMouseDrag)
            {
                int  x = pt.x - m_rcText.left;
                int  y = pt.y - m_rcText.top;
                SetCapture(g_hWnd);
                int nCP, nTrail;
                BOOL isInside;
                // nTrail은 1개의 문자 중앙을 기준으로 클릭한 좌측(false), 우측(true)을 반환한다.
                // 이는 캐럿의 위치를 좌우 위치에 설정하게 된다. 
                // isInsid은 문자열 내부를 클릭한 여부를 반환한다. 문자열 외부를 클릭했을 때 문자열+1에 캐럿을 위치하게 한다.
                if (m_Buffer.XtoCP(x,y, &nCP, &nTrail, isInside, m_Buffer.m_HitClickUp))
                {
                    m_Buffer.m_nClickUp = nCP;
                    if (nTrail)
                        PlaceCaret(nCP + 1);
                    else
                        PlaceCaret(nCP);
                    m_nSelStart = m_nCaret;
                    ResetCaretBlink();
                }
                /*else
                {
                    m_Buffer.m_nClickUp = m_Buffer.m_HitClickUp.textPosition + 1;
                    PlaceCaret(m_Buffer.m_HitClickUp.textPosition + 1);
                }*/
            }

            break;
        }
        return false;
    }
    void TEditBox::DragKeyBegin()
    {
        if (GetKeyState(VK_SHIFT) & 0x8000)
        {
            if (m_bKeyDrag == false)
            {
                if (m_Buffer.CPtoPos(m_nCaret, FALSE, nullptr, nullptr, m_Buffer.m_HitClickDown))
                {
                    ResetCaretBlink();
                }
                m_Buffer.m_nClickDown = m_Buffer.m_HitClickDown.textPosition;;
            }
            m_bKeyDrag = true;
        }
        else
        {
            m_bKeyDrag = false;
            if (m_Buffer.CPtoPos(m_nCaret, FALSE, nullptr, nullptr, m_Buffer.m_HitClickDown))
            {
                ResetCaretBlink();
            }
            m_Buffer.m_nClickDown = m_Buffer.m_HitClickDown.textPosition;
            m_Buffer.m_nClickUp = m_Buffer.m_HitClickDown.textPosition;
            m_Buffer.m_HitClickUp = m_Buffer.m_HitClickDown;
        }
    }
    void TEditBox::DragKeyEnd()
    {
        if (m_bKeyDrag)
        {
            if (m_Buffer.CPtoPos(m_nCaret, FALSE, nullptr, nullptr, m_Buffer.m_HitClickUp))
            {
                m_Buffer.m_nClickUp = m_Buffer.m_HitClickUp.textPosition;
                ResetCaretBlink();
            }
        }
    }
    void TEditBox::Move(TRect tRt)
    {
        SetAttribute(tRt);
        SetDrawList(0.01f, 0.01f, 0.1f, 0.1f, m_pSprite);
        for (auto data : m_pChildList)
        {
            data->Move(tRt);
        }
    }
    bool  TComboBox::Init()
    {
        m_Type = T_CONTROL_LISTBOX;
        return true;
    }
    bool  TComboBox::Frame()
    {
        TWidget::Frame();
        TText* pTextBtn = (TText*)m_pChildList[0];
        TButton* pDownBtn = (TButton*)m_pChildList[1];
        pDownBtn->Frame();

        m_bVisibleListBtn = pDownBtn->m_bToggle;
        if (m_bVisibleListBtn)
        {
            TListBox* pListBtn = (TListBox*)m_pChildList[2];
            pListBtn->Frame();
            if (pListBtn->m_pSelectBtn != nullptr)
            {
                // 선택된 리스트 콤보창에 출력
                TButton* pSelectBtn = (TButton*)pListBtn->m_pSelectBtn;
                TText* pSelectText = (TText*)pSelectBtn->m_pChildList[0];
                pTextBtn->SetText(pSelectText->m_szText.c_str());
            }
            if (m_pParentWidget)
            {
                m_pPostDrawWidget.push_back(pListBtn);
            }
        }
        SetPostDrawList();
        return true;
    }
    bool  TComboBox::Render()
    {
        // TEditBox::Render();
        TWidget* pText = m_pChildList[0];
        pText->Render();

        TWidget* pDownBtn = m_pChildList[1];
        pDownBtn->Render();
        if (m_bVisibleListBtn)
        {
            TListBox* pListBtn = (TListBox*)m_pChildList[2];
            if (m_pParentWidget==nullptr)
            {               
                pListBtn->Render();
            }
        }
        PostDrawList();
        return true;
    }
    bool  TComboBox::Release()
    {
        return TWidget::Release();
    }
    void  TComboBox::SetRect(TRect rt)
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
    bool  TComboBox::Push(TWidget* btn)
    {
        TWidget* pListBtn = m_pChildList[2];
        pListBtn->Push(btn);
        return true;
    }
    bool  TComboBox::Push(std::wstring name)
    {
        TWidget* pListBtn = m_pChildList[2];
        pListBtn->Push(name);
        return true;
    }
    TRect TComboBox::GetNextRect()
    {
        TWidget* pListBtn = m_pChildList[2];
        return pListBtn->GetNextRect();
    }
    void  TComboBox::Move(TVector2D vOffsetPos)
    {        
        TRect tRt = m_rtCollision;
        //tRt += vOffsetPos;
        SetAttribute({vOffsetPos.x, vOffsetPos.y, tRt.w, tRt.h});
        //Move(vOffsetPos);

        TText* pText = (TText*)m_pChildList[0];
        pText->SetAttribute(m_rtCollision);
        //pText->Move(vOffsetPos);
       /* pText->Create({ pRootCtl->m_rtCollision.x1, pRootCtl->m_rtCollision.y1,
            pRootCtl->m_rtCollision.w,    pRootCtl->m_rtCollision.h }, L"Elephant",
            (pRootCtl->m_rtCollision.w < pRootCtl->m_rtCollision.h) ? pRootCtl->m_rtCollision.w / 3 : pRootCtl->m_rtCollision.h / 3);
        pText->SetText({ pRootCtl->m_rtCollision.w, pRootCtl->m_rtCollision.h }, L"select");
        pRootCtl->AddChild(pText);*/

        TButton* pButton = (TButton*)m_pChildList[1];
         pButton->SetAttribute({ m_rtCollision.x2,
                                 m_rtCollision.y1,
                                 max(10.0f,m_rtCollision.h),
                                 m_rtCollision.h });
        //pRootCtl->AddChild(pButton);


         TListBox* pListBox = (TListBox*)m_pChildList[2];
        pListBox->SetAttribute( {   m_rtCollision.x1,  m_rtCollision.y2,
                                    m_rtCollision.w,   m_rtCollision.h * pListBox->m_INumVisibleBtn });
        //pRootCtl->AddChild(pListBox);
    }
    void  TComboBox::Move(TRect tRt)
    {
        SetAttribute(tRt);

        TText* pText = (TText*)m_pChildList[0];
        pText->Move({ m_rtCollision.x1, m_rtCollision.y1, max(10.0f, tRt.w - tRt.h), m_rtCollision.h });

        TButton* pButton = (TButton*)m_pChildList[1];
        pButton->Move({ pText->m_rtCollision.x2,
                                m_rtCollision.y1,
                                max(10.0f,m_rtCollision.h),
                                m_rtCollision.h });

        TListBox* pListBox = (TListBox*)m_pChildList[2];
        pListBox->Move({ m_rtCollision.x1,  m_rtCollision.y2, pText->m_rtCollision.w+ pButton->m_rtCollision.w, m_rtCollision.h * pListBox->m_INumVisibleBtn });
    }
};

namespace TUI
{
    TEditBox* CreateEditBox(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        IDWriteTextFormat* pTextFormat,
        TRect tRt)
    {
        TEditBox* pRootCtl = new TEditBox;
        pRootCtl->Create(pd3dDevice, pImmediateContext);
        pRootCtl->SetAttribute(tRt);
        pRootCtl->SetDrawList(0.01f, 0.01f, 0.1f, 0.1f, pSprite);
        pRootCtl->SetEnabled(true);
        pRootCtl->SetVisible(true);
        pRootCtl->m_Buffer.SetTextLayout(pTextFormat, (float)g_rtClient.right, (float)g_rtClient.bottom, L"");
        return pRootCtl;
    }
    TComboBox* CreateComboBox(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TSprite* pSprite,
        IDWriteTextFormat* pTextFormat,
        TRect tRt,
        UINT iNumVisibleBtn, UINT iNumBtn )
    {
        TComboBox* pRootCtl = new TComboBox;
        pRootCtl->Create(pd3dDevice, pImmediateContext);
        pRootCtl->SetAttribute(tRt);
        pRootCtl->SetDrawList(0.01f, 0.01f, 0.1f, 0.1f, pSprite);
        pRootCtl->SetEnabled(true);
        pRootCtl->SetVisible(true);
        //pRootCtl->m_Buffer.SetTextLayout(pTextFormat, (float)g_rtClient.right, (float)g_rtClient.bottom, L"");

        TText* pText = new TText();
        pText->Create({ pRootCtl->m_rtCollision.x1, pRootCtl->m_rtCollision.y1,
            max(10.0f,tRt.w - tRt.h),    pRootCtl->m_rtCollision.h }, L"Elephant",
            (pRootCtl->m_rtCollision.w < pRootCtl->m_rtCollision.h) ? pRootCtl->m_rtCollision.w / 3 : pRootCtl->m_rtCollision.h / 3);
        pText->SetText({ max(10.0f,tRt.w - tRt.h), pRootCtl->m_rtCollision.h }, L"select");
        pRootCtl->AddChild(pText);

        TButton* pButton = new TButton(I_Sprite.GetPtr(L"DXComboButton"));
        pButton->Create(pd3dDevice, pImmediateContext);
        pButton->SetAttribute({ pText->m_rtCollision.x2,
                                pRootCtl->m_rtCollision.y1,
                                max(10.0f,pRootCtl->m_rtCollision.h),
                                pRootCtl->m_rtCollision.h });
        pRootCtl->AddChild(pButton);


        TWidget* pListBox = CreateListBox(pd3dDevice, pImmediateContext,
            I_Sprite.GetPtr(L"DXListBox"), iNumVisibleBtn, iNumBtn,
            { pRootCtl->m_rtCollision.x1,  pRootCtl->m_rtCollision.y2,
              pText->m_rtCollision.w+ pButton->m_rtCollision.w, pRootCtl->m_rtCollision.h * iNumVisibleBtn });
        pRootCtl->AddChild(pListBox);
        return pRootCtl;
    }

    TEditBox* CreateEditBox(TBasisUI* pBasisUI, TSprite* pSprite, IDWriteTextFormat* pTextFormat, TRect tRt)
    {
        TEditBox* pWidget = CreateEditBox(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite, pTextFormat, tRt);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }
    TComboBox* CreateComboBox(TBasisUI* pBasisUI, TSprite* pSprite, IDWriteTextFormat* pTextFormat, TRect tRt, UINT iNumVisibleBtn, UINT iNumBtn)
    {
        TComboBox* pWidget = CreateComboBox(pBasisUI->m_pd3dDevice, pBasisUI->m_pImmediateContext, pSprite, pTextFormat, tRt, iNumVisibleBtn, iNumBtn);
        pWidget->Add(pBasisUI->m_pPriorityQueue);
        return pWidget;
    }
};