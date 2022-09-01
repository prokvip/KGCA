#include "Sample.h"
bool	Sample::Init()
{     
    m_object.Create(m_pd3dDevice, m_pImmediateContext,
                    L"../../data/kgcabk.bmp",
                    L"../../data/shader/DefaultShape.txt");
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
