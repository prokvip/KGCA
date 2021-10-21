#include "Sample.h"
HRESULT Sample::CreateDepthStencilView()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = Desc.BufferDesc.Width;
	td.Height = Desc.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality= 0;
	hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	svd.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(pTexture,&svd,
		&m_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}
	SAFE_RELEASE(pTexture);

	m_pImmediateContext->OMSetRenderTargets(1,
		&m_pRenderTargetView, m_pDepthStencilView);
	return hr;
}
HRESULT Sample::CreateDepthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	sd.DepthEnable = TRUE;
	sd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_pd3dDevice->CreateDepthStencilState(&sd, &m_pDsvState);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pImmediateContext->OMSetDepthStencilState(m_pDsvState, 0x01);
	return hr;
}
bool		Sample::Init()
{
	CreateDepthStencilView();
	CreateDepthStencilState();
	return true;
}
bool		Sample::Frame() {
	return true;
}
bool		Sample::Render() {
	return true;
}
bool		Sample::Release() 
{
	SAFE_RELEASE(m_pDsvState);
	SAFE_RELEASE(m_pDepthStencilView);
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)