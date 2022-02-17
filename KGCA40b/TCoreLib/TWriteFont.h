#pragma once
#include "TStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
class TWriteFont
{
public:
	ID2D1Factory*			m_pd2dFactory = nullptr;
	IDWriteFactory*			m_pWriteFactory = nullptr;

	ID2D1RenderTarget*		m_pd2dRT = nullptr;
	IDWriteTextFormat*		m_pd2dTextFormat = nullptr;
	ID2D1SolidColorBrush*	m_pd2dColorBrush = nullptr;
public:
	bool   SetRenderTarget(IDXGISurface1* pSurface);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

