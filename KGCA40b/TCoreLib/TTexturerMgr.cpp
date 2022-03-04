#include "TTextureMgr.h"
bool	TTexture::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice,
		filename.c_str(),
		(ID3D11Resource**)m_pTexture.GetAddressOf(),
		m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice,
			filename.c_str(),
			(ID3D11Resource**)m_pTexture.GetAddressOf(),
			m_pSRV.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_pTexture->GetDesc(&m_TextureDesc);
	return true;
}
bool	TTexture::Init()
{
	return true;
}
bool	TTexture::Frame()
{	
	return true;
}
bool	TTexture::Render()
{
	return true;
}
bool	TTexture::Release()
{
	return true;
}
TTexture::TTexture()
{

}
TTexture::~TTexture()
{

}

