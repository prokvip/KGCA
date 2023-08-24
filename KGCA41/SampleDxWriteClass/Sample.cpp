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