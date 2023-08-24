#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
class TWriter
{
public:
	ID2D1Factory* m_pD2DFactory = nullptr;
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pTextFormat = nullptr;
	ID2D1RenderTarget* m_pRT = nullptr;
	ID2D1SolidColorBrush* m_pBrush = nullptr;
public:
	bool Create(IDXGISurface1* pBackBuffer);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool CrateDXWriteRT(IDXGISurface1* pSurface);
};

