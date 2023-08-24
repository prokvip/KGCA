#pragma once
#include "TCore.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class Sample : public TCore
{
	ID2D1Factory*	m_pD2DFactory = nullptr;
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pTextFormat=nullptr;
	ID2D1RenderTarget* m_pRT = nullptr;
	ID2D1SolidColorBrush* m_pBrush = nullptr;
public:
	bool CrateDXWriteRT(IDXGISurface1* pSurface);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

