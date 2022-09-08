#include "Sample.h"
bool	Sample::Init()
{
    m_Object.m_pd3dDevice = m_pd3dDevice;
    m_Object.m_pImmediateContext = m_pImmediateContext;

    m_Object.Init();
    return true;
}
bool Sample::Frame()
{	
    m_Object.Frame();
	return true;
}
bool Sample::Render()
{	
    m_Object.Render();
	return true;
}
bool Sample::Release()
{	
    m_Object.Release();
	return true;
}

GAME_RUN(TestWriter, 800,600)