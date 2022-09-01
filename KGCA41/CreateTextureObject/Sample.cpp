#include "Sample.h"
bool	Sample::Init()
{ 
    m_object.SetDevice(m_pd3dDevice, m_pImmediateContext);
    m_object.Create(L"../../data/4.jpg");
    return true;
}
bool		Sample::Frame()
{
    m_object.Frame();
    return true;
}
bool		Sample::Render()
{
    m_object.Render();
    return true;
}
bool		Sample::Release()
{
    m_object.Render();
    return true;
}


GAME_RUN(CreateTextureObject, 1024, 768)
