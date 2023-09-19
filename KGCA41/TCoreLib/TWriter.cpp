#include "TWriter.h"
#include "TStd.h"
// resize
bool  TWriter::DeleteDxResource()
{
	m_pRT->Release();
	m_pDefaultBrush->Release();
	return true;
}
bool  TWriter::CreateDxResource(IDXGISurface1* pBackBuffer)
{
	if (pBackBuffer)
	{
		if (CrateDXWriteRT(pBackBuffer))
		{
			return true;
		}		
	}
	return true;
}
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
		D2D1::ColorF(0,0,0,1),//D2D1::ColorF(D2D1::ColorF::Yellow),
		&m_pDefaultBrush);
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
			m_pD2DFactory.ReleaseAndGetAddressOf());
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			(IUnknown**)m_pDWriteFactory.ReleaseAndGetAddressOf());
		if (SUCCEEDED(hr))
		{
			m_pDWriteFactory->CreateTextFormat(
				L"±Ã¼­",
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				30,
				L"ko-kr", // en-us
				m_pDefaultTextFormat.ReleaseAndGetAddressOf());
		}
	}

	CreateDxResource(pBackBuffer);
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
bool TWriter::PreRender()
{
	if (m_pRT)
	{
		m_pRT->BeginDraw();
		m_pRT->SetTransform(D2D1::IdentityMatrix());		
	}
	return true;
}
bool TWriter::Render()
{
	if(PreRender())
	{
		m_pDefaultTextFormat->GetFontSize();
		for (int iText = 0; iText < m_TextList.size(); iText++)
		{
			std::wstring text = m_TextList[iText].text;
			D2D1_RECT_F layout = m_TextList[iText].layout;
			m_pDefaultBrush->SetColor(m_TextList[iText].color);
			m_pDefaultBrush->SetOpacity(1.0f);
			m_pRT->DrawText(text.c_str(), text.size(),
				m_pDefaultTextFormat.Get(), &layout, m_pDefaultBrush);
		}

		PostRender();
	}
	return true;
}
bool TWriter::PostRender()
{
	if (m_pRT)
	{		
		m_pRT->EndDraw();
	}
	m_TextList.clear();
	return true;
}
bool TWriter::Release()
{
	DeleteDxResource();
	return true;
}


void TWriter::AddText(std::wstring text,
	float x, float y,
	D2D1::ColorF color)
{
	TTextData textdata;
	textdata.text = text;
	textdata.layout = { x, y,
		(float)g_dwClientWidth,
		(float)g_dwClientHeight };
	textdata.color = color;
	m_TextList.push_back(textdata);
}