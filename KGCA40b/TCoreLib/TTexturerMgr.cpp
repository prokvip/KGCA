#include "TTextureMgr.h"
bool	TTexture::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice,
		filename.c_str(),
		(ID3D11Resource**)&m_pTexture,
		&m_pSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice,
			filename.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pSRV);
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
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	return true;
}
TTexture::TTexture()
{

}
TTexture::~TTexture()
{

}

