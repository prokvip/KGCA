#include "TTexture.h"
bool	TTexture::Init()
{
    return true;
}
bool	TTexture::Frame() {
    return true;
}
bool	TTexture::Render() {
    return true;
}
bool	TTexture::Release() {
    if (m_pTexture)m_pTexture->Release();
    if (m_pTextureSRV)m_pTextureSRV->Release();
    m_pTexture = nullptr;
    m_pTextureSRV = nullptr;
    return true;
}
HRESULT TTexture::Load(
    ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pImmediateContext,
    std::wstring filename)
{
    HRESULT hr;
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pImmediateContext;
    hr = DirectX::CreateWICTextureFromFile(
        pd3dDevice,
        pImmediateContext,
        filename.c_str(),
        (ID3D11Resource**)&m_pTexture,
        &m_pTextureSRV);

    m_pTexture->GetDesc(&m_Desc);
    return hr;
}