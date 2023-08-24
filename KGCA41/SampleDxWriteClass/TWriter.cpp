#include "TWriter.h"
#include "TStd.h"
bool TWriter::CrateDXWriteRT(IDXGISurface1* pSurface)
{
	FLOAT xDpi, yDpi;
	//error C4996: 'ID2D1Factory::GetDesktopDpi': Deprecated. Use DisplayInformation::LogicalDpi for Windows Store Apps or GetDpiForWindow for desktop apps.
	//m_pD2DFactory->GetDesktopDpi(&xDpi, &yDpi);
	float dpi = GetDpiForWindow(g_hWnd);

	D2D1_RENDER_TARGET_PROPERTIES prop;
	ZeroMemory(&prop, sizeof(prop));
	prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	prop.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	prop.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	prop.dpiX = dpi;
	prop.dpiY = dpi;
	prop.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	HRESULT hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(
		pSurface,
		&prop,
		&m_pRT);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&m_pBrush);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool TWriter::Create(IDXGISurface1* pBackBuffer)
{
	HRESULT hr =
		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
			&m_pD2DFactory);
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			(IUnknown**)&m_pDWriteFactory);
		if (SUCCEEDED(hr))
		{
			m_pDWriteFactory->CreateTextFormat(
				L"°íµñ",
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				30,
				L"ko-kr", // en-us
				&m_pTextFormat);
		}
	}

	if (pBackBuffer)
	{
		if (SUCCEEDED(hr))
		{
			if (CrateDXWriteRT(pBackBuffer))
			{
				return true;
			}
		}	
	}
	return false;
}
bool TWriter::Init()
{	
	return true;
}
bool TWriter::Frame()
{
	return true;
}
bool TWriter::Render()
{
	if (m_pRT)
	{
		m_pRT->BeginDraw();
		m_pRT->SetTransform(D2D1::IdentityMatrix());
		std::wstring text = L"kgca";
		D2D1_RECT_F layout = { 0.0f, 0.0f, 800.0f, 600.0f };
		m_pRT->DrawText(text.c_str(), text.size(),
			m_pTextFormat, &layout, m_pBrush);

		std::wstring text1 = L"game";
		D2D1_RECT_F layout1 = { 0.0f, 30.0f, 800.0f, 600.0f };

		m_pRT->DrawText(text1.c_str(), text1.size(),
			m_pTextFormat, &layout1, m_pBrush);
		m_pRT->EndDraw();
	}
	return true;
}
bool TWriter::Release()
{
	if (m_pBrush)m_pBrush->Release();
	if (m_pTextFormat)m_pTextFormat->Release();
	if (m_pRT)m_pRT->Release();
	if (m_pDWriteFactory)m_pDWriteFactory->Release();
	if (m_pD2DFactory)m_pD2DFactory->Release();
	return true;
}
