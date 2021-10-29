#include "TViewDS.h"
ID3D11Texture2D* TViewDS::CreateTexture(UINT Width,UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	ID3D11Texture2D* pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL|D3D11_BIND_SHADER_RESOURCE;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	if (FAILED(hr = g_pd3dDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
	{
		return nullptr;
	}
	return pDSTexture;
}
HRESULT TViewDS::CreateDepthStencilView(UINT Width,
										UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	m_pTexture = CreateTexture(Width, Height);
	if (m_pTexture ==nullptr)
	{
		return E_FAIL;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = g_pd3dDevice->CreateDepthStencilView(m_pTexture, &svd,
		&m_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Desc.Texture2D.MipLevels = 1;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture,
		&Desc, &m_pTextureSRV);
	if (FAILED(hr))
	{
		m_pTexture->Release();
		return hr;
	}
	return hr;
}

bool		TViewDS::Release()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTextureSRV);
	SAFE_RELEASE(m_pDepthStencilView);
	return true;
}