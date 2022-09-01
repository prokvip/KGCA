#include "Sample.h"
bool		Sample::Init()
{   
    TDevice::Init();

    HRESULT hr;
    hr=    DirectX::CreateWICTextureFromFile(
            m_pd3dDevice, 
            m_pImmediateContext, 
            L"../../data/kgcabk.bmp", 
            &m_Texture,
            &m_pTextureSRV);
    return true;
}
bool		Sample::Frame()
{
    TDevice::Frame();
    return true;
}
bool		Sample::Render()
{
    TDevice::Render();
    return true;
}
bool		Sample::Release()
{
    TDevice::Release();

    if (m_Texture) m_Texture->Release();
    if (m_pTextureSRV) m_pTextureSRV->Release();
    return true;
}


GAME_RUN(P2_CreateDevice_0, 1024, 768)
