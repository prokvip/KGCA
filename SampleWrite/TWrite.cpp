#include "TWrite.h"
HRESULT TWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
    HRESULT hr = S_OK;

    UINT dpi = GetDpiForWindow(g_hWnd);
    D2D1_RENDER_TARGET_PROPERTIES props;
    props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    props.pixelFormat.format= DXGI_FORMAT_UNKNOWN;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    props.dpiX = dpi;
    props.dpiY = dpi;
    props.usage= D2D1_RENDER_TARGET_USAGE_NONE;
    props.minLevel= D2D1_FEATURE_LEVEL_DEFAULT;

    hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
        pSurface, 
        &props, 
        &m_pRT);
    if (FAILED(hr)) return hr;

    m_pRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Yellow), 
        &m_pTextBrush);
    if (FAILED(hr)) return hr;
    return S_OK;
}
bool TWrite::Init()
{
    HRESULT hr;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    if (FAILED(hr)) return false;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory), 
        reinterpret_cast<IUnknown**>(&m_pdWriteFactory));
    if (FAILED(hr)) return false;
    
     hr = m_pdWriteFactory->CreateTextFormat(
            L"±Ã¼­",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"ko-kr",//L"en-us",//L"ko-kr",
            &m_pTextFormat
        );
     hr = m_pdWriteFactory->CreateTextFormat(
         L"°íµñ",
         NULL,
         DWRITE_FONT_WEIGHT_NORMAL,
         DWRITE_FONT_STYLE_NORMAL,
         DWRITE_FONT_STRETCH_NORMAL,
         50,
         L"ko-kr",//L"en-us",//L"ko-kr",
         &m_pTextFormat50
     );
    if (FAILED(hr)) return false;
    return true;
}

bool TWrite::Frame()
{
    return false;
}

bool TWrite::Render()
{
    //RECT  rt = { 0, 0, 800, 600 };
    //DrawText(rt, L"KGCA",//m_Timer.m_szTimerString,
    //    D2D1::ColorF(1, 1, 1, 1));
    /*if (m_pRT)
    {
        m_pRT->BeginDraw();
            m_pRT->SetTransform(D2D1::IdentityMatrix());
            TCHAR data[] = L"KGCA";
            RECT rt = { 0,0,800, 600 };
            D2D1_RECT_F rect = D2D1::RectF( rt.left, rt.top, rt.right, rt.bottom);
            m_pTextBrush->SetColor(D2D1::ColorF(1,0,0,1));
            m_pRT->DrawText(data, wcslen(data), 
                m_pTextFormat, rect, m_pTextBrush);
        m_pRT->EndDraw();
    }*/
    return false;
}
bool TWrite::DrawText(RECT rt, 
    const TCHAR* data, D2D1::ColorF color,
    IDWriteTextFormat* pTextFormat)
{
    if (m_pRT)
    {
        m_pRT->BeginDraw();
        m_pRT->SetTransform(D2D1::IdentityMatrix());
        D2D1_RECT_F rect = D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom);
        m_pTextBrush->SetColor(color);
        if (pTextFormat == nullptr)
        {
            m_pRT->DrawText(data, wcslen(data),
                m_pTextFormat, rect, m_pTextBrush);
        }
        else
        {
            m_pRT->DrawText(data, wcslen(data),
                pTextFormat, rect, m_pTextBrush);
        }
        m_pRT->EndDraw();
    }
    return true;
}
bool TWrite::Release()
{
    if (m_pd2dFactory) m_pd2dFactory->Release();
    if (m_pdWriteFactory) m_pdWriteFactory->Release();
    if (m_pRT) m_pRT->Release();
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pTextFormat50) m_pTextFormat50->Release();
    if (m_pTextBrush) m_pTextBrush->Release();
    
    return false;
}
