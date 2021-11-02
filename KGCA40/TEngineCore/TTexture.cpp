#include "TTexture.h"

bool  TTexture::LoadTexture(std::wstring texFileName)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        g_pd3dDevice,
        texFileName.c_str(),
        &m_pTexture,
        &m_pTextureSRV);
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(
            g_pd3dDevice,
            texFileName.c_str(),
            &m_pTexture,
            &m_pTextureSRV);
    }
    if (FAILED(hr)) return false;
    return true;
}
bool TTexture::Init()
{
    return false;
}
bool TTexture::Frame()
{
    return false;
}
bool TTexture::Render()
{
    return false;
}
bool TTexture::Release()
{
    SAFE_RELEASE(m_pTexture);
    SAFE_RELEASE(m_pTextureSRV); 
    return true;
}

TTexture::TTexture()
{
    SAFE_ZERO(m_pTexture);
    SAFE_ZERO(m_pTextureSRV);
}