#include "TWriteFont.h"
bool	TWriteFont::Init()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pd2dFactory);
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			(IUnknown**)(&m_pWriteFactory));
		if (SUCCEEDED(hr))
		{
			hr = m_pWriteFactory->CreateTextFormat(
				L"±Ã¼­",
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				20,
				L"ko-kr", // L"en-us"
				&m_pd2dTextFormat);
		}
	}

	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}
bool    TWriteFont::SetRenderTarget(IDXGISurface1* pSurface)
{
	UINT dpi = GetDpiForWindow(g_hWnd);

	D2D1_RENDER_TARGET_PROPERTIES rtp;
	ZeroMemory(&rtp, sizeof(D2D1_RENDER_TARGET_PROPERTIES));
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,
							D2D1_ALPHA_MODE_PREMULTIPLIED);
	rtp.dpiX = dpi;
	rtp.dpiY = dpi;
	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
		pSurface,
		&rtp,
		&m_pd2dRT);
	return true;
}
bool	TWriteFont::Frame()
{
	if (m_pd2dTextFormat)m_pd2dTextFormat->Release();
	if (m_pd2dColorBrush)m_pd2dColorBrush->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pWriteFactory)m_pWriteFactory->Release();
	if (m_pd2dFactory)m_pd2dFactory->Release();
	return true;
}
bool	TWriteFont::Render() 
{
	return true;
}
bool	TWriteFont::Release() 
{
	return true;
}