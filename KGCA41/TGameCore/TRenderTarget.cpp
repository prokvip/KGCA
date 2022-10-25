#include "TRenderTarget.h"
bool TRenderTarget::Create(ID3D11Device* pd3dDevice,
							ID3D11DeviceContext* pImmediateContext,
							FLOAT fWidth, FLOAT fHeight)
{
	HRESULT hr;	
	m_Viewport.Width = fWidth;
	m_Viewport.Height = fHeight;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	

	m_TexDesc.Width = (UINT)fWidth;
	m_TexDesc.Height = (UINT)fHeight;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TexDesc.SampleDesc.Count = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.ArraySize = 1;

	if (FAILED(hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf())))
	{
		return hr;
	}

	/// <summary>
	/// Depth Buffer
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = fWidth;
	DescDepth.Height = fHeight;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	// 백 버퍼 깊이 및 스텐실 버퍼 생성
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	if (FAILED(hr = pd3dDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
	{
		return hr;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (FAILED(hr = pd3dDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, &m_pDepthStencilView)))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	if (FAILED(hr = pd3dDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, &m_pDsvSRV)))
	{
		return hr;
	}
	
	return true;
}
bool TRenderTarget::Begin(ID3D11DeviceContext* pContext)
{
	/*UINT iNumViewport = 1;
	pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);
	pContext->RSGetViewports(&iNumViewport, m_vpOld);*/

	ID3D11RenderTargetView* pNullRTV = NULL;
	pContext->OMSetRenderTargets(1, &pNullRTV, NULL);
	pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	const FLOAT color[] = { 1, 1, 1, 1 };
	pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH| D3D11_CLEAR_STENCIL, 1.0, 0);
	pContext->RSSetViewports(1, &m_Viewport);
	return true;
}
void TRenderTarget::End(ID3D11DeviceContext* pContext)
{
	pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	pContext->RSSetViewports(1, m_vpOld);
	/*m_pOldRTV->Release();
	m_pOldDSV->Release();*/
}
bool TRenderTarget::Release() {
	return true;
}