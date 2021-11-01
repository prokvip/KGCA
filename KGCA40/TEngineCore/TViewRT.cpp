#include "TViewRT.h"
#include "ScreenGrab.h"
#include <wincodec.h>
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
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
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
	m_pTexture = CreateTexture(Width, Height);
	if (m_pTexture == nullptr)
	{
		return E_FAIL;
	}
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture, NULL, &m_pTextureSRV);
	if (FAILED(hr))
	{
		m_pTexture->Release();
		return hr;
	}
	hr = g_pd3dDevice->CreateRenderTargetView(
		m_pTexture, NULL,
		&m_pRenderTargetView);
	if (FAILED(hr))
	{
		m_pTexture->Release();
		return hr;
	}
	return hr;
}
bool TViewRT::Create(UINT Width, UINT Height)
{
	SetViewPort(Width, Height);
	CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.25f,
		(float)Width / (float)Height);
	if (FAILED(CreateRenderTargetView(Width, Height)))
	{
		return false;
	}
	if (FAILED(m_dxDs.CreateDepthStencilView(Width, Height)))
	{
		return false;
	}
	return false;
}
bool TViewRT::Begin(ID3D11DeviceContext* pContext)
{
	m_nViewPorts = 1;
	pContext->RSGetViewports(&m_nViewPorts, m_vpOld);
	pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);

	float ClearColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f }; //red,green,blue,alpha
	pContext->ClearRenderTargetView(
		this->m_pRenderTargetView, ClearColor);
	pContext->ClearDepthStencilView(
		this->m_dxDs.m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->OMSetRenderTargets(1,
		&this->m_pRenderTargetView, 
		this->m_dxDs.m_pDepthStencilView);
	pContext->RSSetViewports(1, &m_ViewPort);
	return true;
}
bool TViewRT::End(ID3D11DeviceContext* pContext)
{
	pContext->RSSetViewports(m_nViewPorts, m_vpOld);
	pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	SAFE_RELEASE(m_pOldRTV);
	SAFE_RELEASE(m_pOldDSV);
	return true;
}
TMatrix  	TViewRT::CreateProjMatrix(
	float fNear, float fFar, float fFov, float fAspect)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
	return m_matProj;
}
HRESULT TViewRT::SetViewPort(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// Setup the viewport    
	m_ViewPort.Width = Width;
	m_ViewPort.Height = Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	return hr;
}
bool TViewRT::Release()
{
	m_dxDs.Release();
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTextureSRV);
	SAFE_RELEASE(m_pRenderTargetView);
	return true;
}

void TViewRT::Save(ID3D11DeviceContext* pContext,
	               std::wstring saveFileName)
{
	ID3D11Resource* pSource = (ID3D11Resource * )m_pTexture;
	HRESULT hr = DirectX::SaveDDSTextureToFile(
		pContext,
		pSource,
		saveFileName.c_str());

	//HRESULT hr = SaveWICTextureToFile(
	//	pContext,
	//	pSource,
	//	GUID_ContainerFormatJpeg,
	//	saveFileName.c_str());
}
