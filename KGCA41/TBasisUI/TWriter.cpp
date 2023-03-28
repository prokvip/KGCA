#include "TWriter.h"
#include "TInputUI.h"
//https://learn.microsoft.com/ko-kr/windows/win32/directwrite/justification--kerning--and-spacing
	//https://www.codeproject.com/Articles/5277136/Direct2D-Tutorial-Part-2-Basic-Shapes
	// https://learn.microsoft.com/ko-kr/windows/win32/directwrite/introducing-directwrite
	// http://www.charlespetzold.com/blog/2014/01/Character-Formatting-Extensions-with-DirectWrite.html
	// https://learn.microsoft.com/en-us/archive/msdn-magazine/2013/november/directx-factor-who%E2%80%99s-afraid-of-glyph-runs
namespace TUI
{	
	void TWriter::GetFontEnum(IDWriteFactory* pDWriteFactory)
	{
		HRESULT hr;
		ComPtr<IDWriteFontCollection> pFontCollection = NULL;

		hr = pDWriteFactory->GetSystemFontCollection(pFontCollection.GetAddressOf());

		UINT32 familyCount = 0;

		// Get the number of font families in the collection.
		if (SUCCEEDED(hr))
		{
			familyCount = pFontCollection->GetFontFamilyCount();
		}

		for (UINT32 i = 0; i < familyCount; ++i)
		{
			ComPtr<IDWriteFontFamily> pFontFamily = NULL;

			// Get the font family.
			if (SUCCEEDED(hr))
			{
				hr = pFontCollection->GetFontFamily(i, pFontFamily.GetAddressOf());
			}
			ComPtr<IDWriteLocalizedStrings> pFamilyNames = NULL;

			// Get a list of localized strings for the family name.
			if (SUCCEEDED(hr))
			{
				hr = pFontFamily->GetFamilyNames(pFamilyNames.GetAddressOf());
			}

			UINT32 index = 0;
			BOOL exists = false;

			wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

			if (SUCCEEDED(hr))
			{
				// Get the default locale for this user.
				int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

				// If the default locale is returned, find that locale name, otherwise use "en-us".
				if (defaultLocaleSuccess)
				{
					hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);
				}
				if (SUCCEEDED(hr) && !exists) // if the above find did not find a match, retry with US English
				{
					hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);
				}
			}

			// If the specified locale doesn't exist, select the first on the list.
			if (!exists)
				index = 0;

			UINT32 length = 0;

			// Get the string length.
			if (SUCCEEDED(hr))
			{
				hr = pFamilyNames->GetStringLength(index, &length);
			}

			// Allocate a string big enough to hold the name.
			wchar_t* name = new (std::nothrow) wchar_t[length + 1];
			if (name == NULL)
			{
				hr = E_OUTOFMEMORY;
			}

			// Get the family name.
			if (SUCCEEDED(hr))
			{
				hr = pFamilyNames->GetString(index, name, length + 1);
			}
			if (SUCCEEDED(hr))
			{
				// Print out the family name.
				wprintf(L"%s\n", name);
			}

			pFontFamily.Reset();
			pFamilyNames.Reset();

			delete[] name;
		}
	}
	bool TWriter::Set(IDXGISurface1* dxgiSurface)
	{
		m_pTextColor.Reset();
		m_StrokeBrush.Reset();
		m_FillBrush.Reset();	

		HRESULT hr = S_OK;
		// 중요 : 디바이스 생성 플래그 지정해야 한다. 
		// UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		// 3d -> 2D 연동
		D2D1_RENDER_TARGET_PROPERTIES props;
		ZeroMemory(&props, sizeof(props));
		props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
		props.dpiX = 96;
		props.dpiY = 96;
		props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
		props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
		hr = m_d2dFactory->CreateDxgiSurfaceRenderTarget(dxgiSurface,&props,m_d2dRT.GetAddressOf());
		hr = m_d2dRT->CreateSolidColorBrush({ 0,0,0,1 }, m_pTextColor.GetAddressOf());
		hr = m_d2dRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), m_StrokeBrush.GetAddressOf());
		hr = m_d2dRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), m_FillBrush.GetAddressOf());
		// set antialiasing mode
		m_d2dRT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
		return true;
	}
	bool TWriter::Init()
	{
		HRESULT hr;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, m_d2dFactory.GetAddressOf());
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)m_pDWriteFactory.GetAddressOf());

		GetFontEnum(m_pDWriteFactory.Get());

		hr = m_pDWriteFactory->CreateTextFormat(
			L"고딕",//L"고딕",//L"Gabriola", //L"고딕",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			16,
			L"en-us",//, // L"ko-kr",//
			m_pTextFormat.ReleaseAndGetAddressOf());

		m_pStrokeStyle = CreateStrokeStyle();
		m_szDefaultText = L"KGCA Game Academy";
		return true;
	}
	bool TWriter::Frame() {
		return true;
	}
	bool TWriter::Render()
	{
		Draw(0, 0, m_szDefaultText, { 1,0,0,1 });
		return true;
	}
	bool TWriter::Draw(float x, float y, std::wstring text, D2D1_COLOR_F color)
	{
		m_d2dRT->BeginDraw();
		//m_d2dRT->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
		D2D1_RECT_F rt = { x, y,(float)g_rtClient.right,(float)g_rtClient.bottom };
		m_pTextColor->SetColor(color);
		m_pTextColor->SetOpacity(1.0f);

		//m_pTextColor->SetTransform()
		m_d2dRT->DrawText(text.c_str(), (UINT32)text.size(), m_pTextFormat.Get(), rt, m_pTextColor.Get());

		D2D1_RECT_F rectangle = rt;
		m_d2dRT->DrawRectangle(rectangle, m_pTextColor.Get(), 1.0f, m_pStrokeStyle.Get());
		m_d2dRT->EndDraw();
		return true;
	}
	bool TWriter::Draw(D2D1_RECT_F rt, std::wstring text, D2D1_COLOR_F color)
	{
		m_d2dRT->BeginDraw();
		//m_d2dRT->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));	
		m_pTextColor->SetColor(color);
		m_pTextColor->SetOpacity(1.0f);
		//m_pTextColor->SetTransform()	
		//m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		//DWRITE_TRIMMING Trimming = { DWRITE_TRIMMING_GRANULARITY_WORD, 0, 0 };
		//m_pTextFormat->SetTrimming(&Trimming, nullptr);
		m_d2dRT->DrawText(text.c_str(), text.size(), m_pTextFormat.Get(), rt, m_pTextColor.Get());

		//m_d2dRT->DrawRectangle(rt,	m_pTextColor,1.0f,m_pStrokeStyle.Get());
		m_d2dRT->EndDraw();
		return true;
	}
	bool TWriter::Release()
	{		
		m_pTextFormat.Reset();
		m_pTextColor.Reset();
		m_FontFace.Reset();
		m_StrokeBrush.Reset();
		m_FillBrush.Reset();
		m_pStrokeStyle.Reset();
		m_pTextLayout.Reset();		
		m_d2dRT.Reset();
		m_d2dFactory.Reset();
		m_pDWriteFactory.Reset();
		return true;
	}
	bool TWriter::ResetDevice()
	{
		m_pTextColor.Reset();
		m_StrokeBrush.Reset();
		m_FillBrush.Reset();
		m_d2dRT.Reset();
		return true;
	}
	void TWriter::DrawLine()
	{
		m_StrokeBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
		m_d2dRT->DrawLine(
			D2D1::Point2F(10.0f, 40.0f),
			D2D1::Point2F(110.0f, 40.0f),
			m_StrokeBrush.Get(),
			8.0f,
			stroke.Get());
	}
	void TWriter::DrawRectangle(D2D1_RECT_F rt)
	{
		m_d2dRT->BeginDraw();
		ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
		const D2D1_RECT_F rect = rt;// D2D1::RectF(10, 10, 210, 160);
		m_d2dRT->DrawRectangle(rect, m_StrokeBrush.Get(), 1.0f, stroke.Get());
		m_d2dRT->FillRectangle(rect, m_FillBrush.Get());
		m_d2dRT->EndDraw();
	}
	void TWriter::DrawRoundedRectangle()
	{
		ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
		const D2D1_RECT_F rect = D2D1::RectF(10, 10, 210, 160);
		const D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, 20.f, 20.f);
		m_d2dRT->DrawRoundedRectangle(roundedRect, m_StrokeBrush.Get(), 16.0f, stroke.Get());
		m_d2dRT->FillRoundedRectangle(roundedRect, m_FillBrush.Get());
	}
	void TWriter::DrawCircle()
	{
		ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
		const D2D1_ELLIPSE ell = D2D1::Ellipse(D2D1::Point2F(100.0f, 100.0f), 90, 90);
		m_d2dRT->DrawEllipse(ell, m_StrokeBrush.Get(), 16.0f, stroke.Get());
		m_d2dRT->FillEllipse(ell, m_FillBrush.Get());
	}
	void TWriter::DrawTriangle()
	{
		ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
		ComPtr<ID2D1PathGeometry> geometry = GenTriangleGeometry(D2D1::Point2F(110, 10), D2D1::Point2F(210, 140), D2D1::Point2F(10, 140));
		m_d2dRT->DrawGeometry(geometry.Get(), m_StrokeBrush.Get(), 16.0f, stroke.Get());
		m_d2dRT->FillGeometry(geometry.Get(), m_FillBrush.Get());
	}
	HRESULT TWriter::SetText(D2D1_POINT_2F size, const wchar_t* text, D2D1::ColorF Color)
	{
		HRESULT hr = S_OK;
		m_cTextLength = (UINT32)wcslen(text);
		m_wszText.clear();
		m_wszText = text;

		m_pTextLayout.ReleaseAndGetAddressOf();

		hr = m_pDWriteFactory->CreateTextLayout(
			m_wszText.c_str(),
			m_cTextLength,
			m_pTextFormat.Get(),
			size.x,
			size.y,
			m_pTextLayout.GetAddressOf());

		ComPtr<IDWriteTypography> pTypography = NULL;
		if (SUCCEEDED(hr))
		{
			hr = m_pDWriteFactory->CreateTypography(pTypography.GetAddressOf());
		}
		DWRITE_FONT_FEATURE fontFeature = { DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7,1 };
		if (SUCCEEDED(hr))
		{
			hr = pTypography->AddFontFeature(fontFeature);
		}
		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };
		if (SUCCEEDED(hr))
		{
			hr = m_pTextLayout->SetTypography(pTypography.Get(), textRange);
		}
		/*if (SUCCEEDED(hr))
		{
			hr = m_pTextLayout->SetUnderline(m_fontUnderline, textRange);
		}*/

		////https://learn.microsoft.com/ko-kr/windows/win32/directwrite/how-to-perform-hit-testing-on-a-text-layout
		//if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH)
		//{
		//	//IDWriteTextAnalyzer* textAnalyzer;
		//	//m_pDWriteFactory->CreateTextAnalyzer(&textAnalyzer);	
		//	m_pTextLayout->GetMetrics(&tm);
		//	FLOAT  x = I_InputUI.m_ptPos.x;
		//	m_pTextLayout->HitTestPoint(
		//		I_InputUI.m_ptPos.x, I_InputUI.m_ptPos.y,
		//		&isTrailingHit, &isInside, &hitTestMetrics);
		//	if (isInside == TRUE)
		//	{
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
	HRESULT TWriter::DrawTextLayout(D2D1_POINT_2F pos, D2D1::ColorF Color)
	{
		m_d2dRT->BeginDraw();
		m_pTextColor->SetColor(Color);
		//https://learn.microsoft.com/ko-kr/windows/win32/directwrite/how-to-perform-hit-testing-on-a-text-layout
		if (I_InputUI.GetKey(VK_LBUTTON) == KEY_PUSH)
		{
			//IDWriteTextAnalyzer* textAnalyzer;
			//m_pDWriteFactory->CreateTextAnalyzer(&textAnalyzer);	
			m_pTextLayout->GetMetrics(&tm);

			FLOAT  x = I_InputUI.m_ptPos.x - pos.x;
			FLOAT  y = I_InputUI.m_ptPos.y - pos.y;
			m_pTextLayout->HitTestPoint(
				x, y,
				&isTrailingHit, &isInside, &hitTestMetrics);
			if (isInside == TRUE)
			{
				m_pTextLayout->GetUnderline(hitTestMetrics.textPosition, &underline);
				//DWRITE_TEXT_RANGE textRange = { hitTestMetrics.textPosition, 1 };
				//m_pTextLayout->SetUnderline(!underline, textRange);
				m_textRange.startPosition = hitTestMetrics.textPosition;
				m_textRange.length = 1;
				m_wszText.erase(hitTestMetrics.textPosition, 1);
			}

		}
		if (isInside == TRUE)
		{
			FLOAT pointX;
			FLOAT pointY;
			HRESULT hr = m_pTextLayout->HitTestTextPosition(m_textRange.startPosition, isTrailingHit,
				&pointX, &pointY, &hitTestMetrics);

			UINT32 maxHitTestMetricsCount = 255;
			UINT32 actualHitTestMetricsCount;
			hr = m_pTextLayout->HitTestTextRange((UINT32)m_textRange.startPosition,
				(UINT32)m_wszText.size(),
				0.0f,
				0.0f,
				&hitTestMetrics,
				maxHitTestMetricsCount,
				&actualHitTestMetricsCount);

			//m_wszText.erase(std::remove(m_wszText.begin(), m_wszText.end(), L'.'), m_wszText.end());			
			//auto data = m_wszText.at(hitTestMetrics.textPosition);
			//m_wszText.erase(m_wszText.at(hitTestMetrics.textPosition));
			m_pTextLayout->SetUnderline(TRUE, m_textRange);
		}
		m_d2dRT->DrawTextLayout(pos, m_pTextLayout.Get(), m_pTextColor.Get());
		m_d2dRT->EndDraw();
		return S_OK;
	}
	D2D1_POINT_2F TWriter::GetTextSize(std::wstring text, std::wstring font, UINT iFontSize)
	{
		D2D1_POINT_2F ret = { 0, 0 };
		ComPtr<IDWriteTextFormat> pTextFormat;
		HRESULT hr = m_pDWriteFactory->CreateTextFormat(
			font.c_str(),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(float)iFontSize,
			L"en-us",//L"ko-kr",
			pTextFormat.GetAddressOf());
		if (FAILED(hr)) {
			return ret;
		}

		ComPtr<IDWriteTextLayout> pTextLayout;
		hr = m_pDWriteFactory->CreateTextLayout(
			text.c_str(),
			(UINT32)text.size(),
			pTextFormat.Get(),
			1000.0f,
			1000.0f,
			pTextLayout.GetAddressOf());
		if (FAILED(hr)) {
			return ret;
		}

		DWRITE_TEXT_METRICS     TextMetrics;
		pTextLayout->GetMetrics(&TextMetrics);
		ret = { TextMetrics.width, TextMetrics.height };
		return ret;
	}
	ComPtr<ID2D1StrokeStyle> TWriter::CreateStrokeStyle()
	{
		ComPtr<ID2D1StrokeStyle> strokeStyle;

		HRESULT hr = m_d2dFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_ROUND,
				D2D1_CAP_STYLE_ROUND,
				D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_ROUND,
				0.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			strokeStyle.GetAddressOf());
		return strokeStyle;
	}
	ComPtr<ID2D1PathGeometry> TWriter::GenTriangleGeometry(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3)
	{
		ID2D1GeometrySink* pSink = NULL;
		HRESULT hr = S_OK;
		ComPtr<ID2D1PathGeometry> m_pPathGeometry;
		// Create a path geometry.
		if (SUCCEEDED(hr))
		{
			hr = m_d2dFactory->CreatePathGeometry
			(m_pPathGeometry.ReleaseAndGetAddressOf());

			if (SUCCEEDED(hr))
			{
				// Write to the path geometry using the geometry sink.
				hr = m_pPathGeometry->Open(&pSink);
				if (SUCCEEDED(hr))
				{
					pSink->BeginFigure(pt1, D2D1_FIGURE_BEGIN_FILLED);
					pSink->AddLine(pt2);
					pSink->AddLine(pt3);
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
					hr = pSink->Close();
				}
				pSink->Release();
			}
		}
		return m_pPathGeometry;
	}
};