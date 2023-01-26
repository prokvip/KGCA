#pragma once 
#include "TBasisUIStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
namespace TUI
{
	class TBASISUI_API TWriter : public TSingleton<TWriter>
	{
		friend class TSingleton<TWriter>;
	public:
		ComPtr<ID2D1Factory>			m_d2dFactory;
		ComPtr<IDWriteFactory>			m_pDWriteFactory;
		ComPtr<ID2D1RenderTarget>		m_d2dRT;
		ComPtr<IDWriteTextFormat>		m_pTextFormat;
		ComPtr<ID2D1SolidColorBrush>	m_pTextColor;
		ComPtr<IDWriteFontFace>			m_FontFace;
		std::wstring					m_szDefaultText;
		ComPtr<ID2D1SolidColorBrush>	m_StrokeBrush;
		ComPtr<ID2D1SolidColorBrush>	m_FillBrush;
		ComPtr<ID2D1StrokeStyle>		m_pStrokeStyle;
		ComPtr<IDWriteTextLayout>		m_pTextLayout;
	public:
		virtual bool		Init(); // 초기화
		virtual bool		Frame();// 실시간 계산
		virtual bool		Render();// 실시간 랜더링
		virtual bool		Release();// 소멸 및 삭제
		virtual bool		ResetDevice(); // RTV 갱신시 사용됨.
	public:
		bool		Set(IDXGISurface1* dxgiSurface);
		bool		Draw(float x, float y, std::wstring text, D2D1_COLOR_F color = { 0,0,0,1 });
		bool		Draw(D2D1_RECT_F rt, std::wstring text, D2D1_COLOR_F color = { 0,0,0,1 });
		D2D1_POINT_2F GetTextSize(std::wstring text, std::wstring font, UINT iFontSize);
		ComPtr<ID2D1StrokeStyle> CreateStrokeStyle();
		void DrawLine();
		void DrawRectangle(D2D1_RECT_F rect);
		void DrawRoundedRectangle();
		void DrawCircle();
		void DrawTriangle();
		ComPtr<ID2D1PathGeometry> GenTriangleGeometry(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3);
		void GetFontEnum(IDWriteFactory* pDWriteFactory);
	public:
		DWRITE_HIT_TEST_METRICS hitTestMetrics;
		BOOL isTrailingHit;
		BOOL isInside;
		DWRITE_TEXT_METRICS tm;
		BOOL underline = FALSE;
		DWRITE_TEXT_RANGE textRange;
		DWRITE_TEXT_RANGE m_textRange;
		std::wstring			m_wszText;
		UINT32					m_cTextLength;
		BOOL					m_fontUnderline = FALSE;
		HRESULT			SetText(D2D1_POINT_2F pos, const wchar_t* text, D2D1::ColorF Color = { 0,0,0,1 });
		HRESULT			DrawTextLayout(D2D1_POINT_2F pos, D2D1::ColorF Color = { 0,0,0,1 });
	private:
		TWriter() {}
	public:
		~TWriter() {}
	};

#define I_Writer TWriter::GetInstance()
};