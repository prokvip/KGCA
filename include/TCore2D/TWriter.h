#pragma once 
#include "TStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
class TWriter
{
public:
	ID2D1Factory* m_d2dFactory;
	IDWriteFactory* m_pDWriteFactory;
	ID2D1RenderTarget* m_d2dRT;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pTextColor;
	std::wstring		m_szDefaultText;
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();
public:
	bool		Set(IDXGISurface1* dxgiSurface);
	bool		Draw(float x, float y, std::wstring text, D2D1_COLOR_F color = { 0,0,0,1 });
};

