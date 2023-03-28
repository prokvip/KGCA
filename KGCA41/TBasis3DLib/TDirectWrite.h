#pragma once
#include "TBasisStd.h"

class TDirectWrite
{
public:
	float					m_fDPIScale;    
	FLOAT					m_fDPI;
	HWND					m_hWnd;
	ID2D1RenderTarget*		m_pRT;
    ID2D1Factory*			m_pD2DFactory;
    ID2D1SolidColorBrush*	m_pBlackBrush;
    IDWriteFactory*			m_pDWriteFactory;
    IDWriteTextFormat*		m_pTextFormat;   
	IDWriteTextLayout*		m_pTextLayout;
public:
	DWRITE_FONT_WEIGHT		m_fontWeight;
    DWRITE_FONT_STYLE		m_fontStyle;
    BOOL					m_fontUnderline;
    wstring					m_wszFontFamily;
    float					m_fontSize;
	wstring					m_wszText;
    UINT32					m_cTextLength;
public:
	// 초기화
	bool			Set(HWND hWnd, int iWidth, int iHeight,IDXGISurface1*	m_pSurface);
    bool			Init();
	// 랜더링
	bool			Begin();
	HRESULT			DrawText(RECT rc, TCHAR* pText, D2D1::ColorF Color= D2D1::ColorF( 1,0,0,1 ) );
	HRESULT			DrawText(D2D1_POINT_2F origin, D2D1::ColorF Color= D2D1::ColorF( 1,0,0,1 ) );
	bool			End();
	// 전체 소멸
	bool			Release();	
	// 객체 생성 및 소멸
    HRESULT			CreateDeviceIndependentResources();
    void			DiscardDeviceIndependentResources();    
	HRESULT			CreateDeviceResources(IDXGISurface1*	m_pSurface);
    void			DiscardDeviceResources();
public:
	HRESULT			SetText(D2D1_POINT_2F pos,  const wchar_t *text,D2D1::ColorF Color);
	HRESULT			SetFont(const wchar_t *fontFamily);
    HRESULT			SetFontSize(float size);
    HRESULT			SetBold(bool bold);
    HRESULT			SetItalic(bool italic);
    HRESULT			SetUnderline(bool underline);

	// 화면 사이즈 변경
	void			OnResize(UINT width, UINT height, IDXGISurface1*pSurface   );
public:
    TDirectWrite();
    ~TDirectWrite();
    
};
