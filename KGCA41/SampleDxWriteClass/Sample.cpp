#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;
bool Sample::Init()
{
	HRESULT hr;
	m_font.Init();

	if (m_pSwapChain)
	{
		IDXGISurface1* pBackBuffer;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),
			(LPVOID*)&pBackBuffer);
		if (SUCCEEDED(hr))
		{		
			m_font.Create(pBackBuffer);
		}
		if (pBackBuffer) pBackBuffer->Release();
	}	
	
	m_font.AddText(L"대한민국", 10, 100, {1.0f, 0.0f, 0.0f, 1.0f});
	m_font.AddText(L"kgca2", 10, 150,{0.0f, 1.0f, 0.0f, 1.0f});
	m_font.AddText(L"만세", 10, 200,{0.0f, 0.0f, 1.0f, 1.0f});

	return true;
}
bool Sample::Frame()
{
	return true;
}
bool Sample::Render()
{
	m_font.Render();
	return true;
}
bool Sample::Release()
{
	m_font.Release(); 
	return true;
}

TGAME(L"kgca", 800, 600)