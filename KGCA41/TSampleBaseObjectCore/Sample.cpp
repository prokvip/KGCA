#include "Sample.h"
bool	Sample::Init()
{
    m_Plane.m_pd3dDevice = m_pd3dDevice;
    m_Plane.m_pImmediateContext = m_pImmediateContext;
    m_Plane.Init();
    m_Plane.Create(L"../../data/shader/DefaultShape.txt",
        L"../../data/shader/DefaultShape.txt");
    return true;
}
bool Sample::Frame()
{	
    //m_Plane.UpdateVertexBuffer();
    m_Plane.Frame();
	return true;
}
bool Sample::Render()
{	
    m_Plane.Render();
	return true;
}
bool Sample::Release()
{	
    m_Plane.Release();
	return true;
}

GAME_RUN(TestWriter, 800,600)