#include "Sample.h"
//C:\Users\Administrator\Documents\Visual Studio 2019\My Exported Templates
bool Sample::Init()
{	
	m_Writer.Init();

	IDXGISurface1* m_pBackBuffer;
		m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),
			(void**)&m_pBackBuffer);
		m_Writer.Set(m_pBackBuffer);
	m_pBackBuffer->Release();
	return true;
}
bool Sample::Frame()
{	
	m_Writer.Frame();
	return true;
}
bool Sample::Render()
{	
	m_Writer.Render();
	return true;
}
bool Sample::Release()
{	
	m_Writer.Release();
	return true;
}

GAME_RUN(TestWriter, 800,600)