#include "TViewRT.h"
ID3D11Texture2D* TViewRT::CreateTexture(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = Width;
	td.Height = Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	hr = g_pd3dDevice->CreateTexture2D(&td, NULL, &pTexture);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return pTexture;
}
HRESULT TViewRT::SetRenderTargetView(ID3D11Texture2D* pBackBuffer)
{
	HRESULT hr = S_OK;
	
	hr = g_pd3dDevice->CreateRenderTargetView(
		pBackBuffer, NULL,
		&m_pRenderTargetView);
	if (FAILED(hr))
	{
		pBackBuffer->Release();
		return hr;
	}
	pBackBuffer->Release();
	return hr;
}
HRESULT TViewRT::CreateRenderTargetView(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	m_pDSTexture = CreateTexture(Width, Height);
	if (m_pDSTexture == nullptr)
	{
		return E_FAIL;
	}
	hr = g_pd3dDevice->CreateRenderTargetView(
		m_pDSTexture, NULL,
		&m_pRenderTargetView);
	if (FAILED(hr))
	{
		m_pDSTexture->Release();
		return hr;
	}
	return hr;
}
bool TViewRT::Release()
{
	SAFE_RELEASE(m_pDSTexture);
	SAFE_RELEASE(m_pRenderTargetView);
	return true;
}