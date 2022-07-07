#include "TDxRT.h"
#include "TDxState.h"

void TDxRT::Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
	FLOAT MinDepth, FLOAT MaxDepth)
{
	m_vp.TopLeftX = TopLeftX;
	m_vp.TopLeftY = TopLeftY;
	m_vp.Width = Width;
	m_vp.Height = Height;
	m_vp.MinDepth = MinDepth;
	m_vp.MaxDepth = MaxDepth;
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)XM_PI * 0.25f, Width / Height, 0.1f, 1000.0f);
}
HRESULT TDxRT::CreateCubeMap(ID3D11Device* pd3dDevice, FLOAT fWidth, FLOAT fHeight)
{
	HRESULT hr;
	Set(pd3dDevice, 0, 0, fWidth, fHeight, 0.0f, 1.0f);

	// 랜더타켓용 텍스처 생성
	D3D11_TEXTURE2D_DESC TexDesc;
	ZeroMemory(&TexDesc, sizeof(D3D11_TEXTURE2D_DESC));
	TexDesc.Width = (UINT)fWidth;
	TexDesc.Height = (UINT)fHeight;
	TexDesc.MipLevels = 1;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.ArraySize = 6;
	TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	V(pd3dDevice->CreateTexture2D(&TexDesc, NULL, m_pTexture.GetAddressOf()));
	if (m_pTexture != nullptr)
	{
		m_pTexture->GetDesc(&m_TexDesc);
	}

	// 랜더타켓용 뷰 생성
	D3D11_RENDER_TARGET_VIEW_DESC RtvDesc;
	ZeroMemory(&RtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	RtvDesc.Texture2DArray.ArraySize = 6;
	V(pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), &RtvDesc, m_pRenderTargetView.GetAddressOf()));

	// 쉐이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
	ZeroMemory(&SrvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SrvDesc.Format = TexDesc.Format;
	SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SrvDesc.Texture2DArray.MipLevels = 1;
	V(pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &SrvDesc, m_pSRV.GetAddressOf()));


	// 깊이 스텐실 뷰 텍스처 생성
	ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DSDesc;
	ZeroMemory(&DSDesc, sizeof(D3D11_TEXTURE2D_DESC));
	DSDesc.Width = (UINT)fWidth;
	DSDesc.Height = (UINT)fHeight;
	DSDesc.MipLevels = 1;
	DSDesc.ArraySize = 6;
	DSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSDesc.SampleDesc.Count = 1;
	DSDesc.SampleDesc.Quality = 0;
	DSDesc.Usage = D3D11_USAGE_DEFAULT;
	DSDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DSDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	V(pd3dDevice->CreateTexture2D(&DSDesc, NULL, pDSTexture.GetAddressOf()));

	// 깊이 스텐실 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC DsvDesc;
	ZeroMemory(&DsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DsvDesc.Format = DSDesc.Format;
	DsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	DsvDesc.Texture2DArray.ArraySize = 6;
	V(pd3dDevice->CreateDepthStencilView(pDSTexture.Get(), &DsvDesc, &m_pDepthStencilView));
	return S_OK;
}
HRESULT TDxRT::Create(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height, DXGI_FORMAT dsTexFormat)
{
	HRESULT hr = S_OK;
	Set(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

	m_TexDesc.Width = (UINT)Width;
	m_TexDesc.Height = (UINT)Height;
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


	// 백버퍼의 크기가 변경되면 반드시 깊이스텐실 뷰어도 다시 작성되어야 한다.		
	if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height), dsTexFormat))
	{
		return hr;
	}
	return hr;
}
HRESULT TDxRT::UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height, DXGI_FORMAT dsTexFormat)
{
	HRESULT hr;
	if (m_pDepthStencilView != nullptr)
	{
		m_pDepthStencilView.ReleaseAndGetAddressOf();
	}
	if (dsTexFormat != m_DSTexFormat)
	{
		m_DSTexFormat = dsTexFormat;
	}
	m_vp.Width = (FLOAT)Width;
	m_vp.Height = (FLOAT)Height;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = m_DSTexFormat;//DXGI_FORMAT_R24G8_TYPELESS
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;

	// 백 버퍼 깊이 및 스텐실 버퍼 생성
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (DescDepth.Format == DXGI_FORMAT_R24G8_TYPELESS || DescDepth.Format == DXGI_FORMAT_D32_FLOAT)
	{
		// 깊이맵 전용 깊이맵 생성
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	}

	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
	{
		return hr;
	}

	///// 쉐이더 리소스 생성 : 깊이 맵 쉐도우에서 사용한다. ///
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	switch (DescDepth.Format)
	{
	case DXGI_FORMAT_R32_TYPELESS:
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	}
	if (srvDesc.Format == DXGI_FORMAT_R32_FLOAT || srvDesc.Format == DXGI_FORMAT_R24_UNORM_X8_TYPELESS)
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		if (FAILED(hr = pDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, &m_pDsvSRV)))
		{
			return hr;
		}
	}

	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	if (FAILED(hr = pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, &m_pDepthStencilView)))
	{
		return hr;
	}
	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);
	return hr;
}
void TDxRT::Apply(ID3D11DeviceContext* pImmediateContext,
	ID3D11RenderTargetView* pRTV,
	ID3D11DepthStencilView* pDepthStencilView,
	bool bDepthClear, bool bStencilClear)
{
	assert(pImmediateContext);
	assert(pRTV);
	ID3D11RenderTargetView* pNullRTV = NULL;
	pImmediateContext->OMSetRenderTargets(1, &pNullRTV, NULL);

	if (pRTV != NULL)
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->OMSetRenderTargets(1, &pRTV, pDepthStencilView);
		else
			pImmediateContext->OMSetRenderTargets(1, &pRTV, m_pDepthStencilView.Get());
	}
	else
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), pDepthStencilView);
		else
			pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	}
	if (bDepthClear)
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
		else
			pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);

	}
	if (bStencilClear)
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0, 0);
		else
			pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
	}
	pImmediateContext->RSSetViewports(1, &m_vp);
}
bool	TDxRT::Begin(ID3D11DeviceContext* pContext, TVector4 vColor,
	bool bTargetClear, bool bDepthClear, bool bStencilClear)
{
	m_nViewPorts = 1;
	pContext->RSGetViewports(&m_nViewPorts, m_vpOld);
	pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);

	ID3D11RenderTargetView* pNullRTV = NULL;
	pContext->OMSetRenderTargets(1, &pNullRTV, NULL);
	pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	Clear(pContext, vColor, bTargetClear, bDepthClear, bStencilClear);
	pContext->RSSetViewports(1, &m_vp);
	return true;
}
bool	TDxRT::Clear(ID3D11DeviceContext* pContext, TVector4 vColor,
	bool bTargetClear, bool bDepthClear, bool bStencilClear)
{
	if (bTargetClear)
	{
		const FLOAT color[] = { vColor.x, vColor.y, vColor.z, vColor.w };
		pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	}
	if (bDepthClear)
	{
		pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	}
	if (bStencilClear)
	{
		pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
	}
	pContext->RSSetViewports(1, &m_vp);
	return true;
}
bool	TDxRT::End(ID3D11DeviceContext* pContext)
{
	pContext->RSSetViewports(m_nViewPorts, m_vpOld);
	pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	SAFE_RELEASE(m_pOldRTV);
	SAFE_RELEASE(m_pOldDSV);
	return true;
}
HRESULT TDxRT::SaveFile(ID3D11DeviceContext* pContext, T_STR name)
{
	HRESULT hr = S_OK;
	// 랜더타켓뷰에서 텍스처 파일로 저장	
	if (m_pTexture != nullptr)
	{
		if (TTextureMgr::SaveFile(pContext, m_pTexture.Get(), name))
		{
			return hr;
		}
	}
	else if (m_pSRV != nullptr)
	{
		ID3D11Resource* resource;
		m_pSRV.Get()->GetResource(&resource);

		ID3D11Texture2D* texture;
		if (FAILED(hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&texture))))
		{
			return hr;
		}
		hr = TTextureMgr::SaveFile(pContext, texture, name);
		texture->Release();
	}
	else if (m_pRenderTargetView != nullptr)
	{
		ComPtr<ID3D11Resource>       resource;
		ComPtr<ID3D11Texture2D>		 texture;
		m_pRenderTargetView->GetResource(resource.GetAddressOf());
		if (FAILED(hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(texture.GetAddressOf()))))
		{
			return hr;
		}
		hr = TTextureMgr::SaveFile(pContext, texture.Get(), name + L"RTV");
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// 백버퍼 저장 
//--------------------------------------------------------------------------------------
HRESULT TDxRT::SaveBackBuffer(ID3D11DeviceContext* pContext, T_STR name)
{
	HRESULT hr = S_OK;
	ID3D11Resource* backbufferRes;
	m_pRenderTargetView.Get()->GetResource(&backbufferRes);


	ID3D11Texture2D* tex = nullptr;
	if (FAILED(hr = backbufferRes->QueryInterface(__uuidof(ID3D11Texture2D),
		reinterpret_cast<LPVOID*>(&tex))))
	{

	}
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	backbufferRes->Release();


	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = 0;
	texDesc.CPUAccessFlags = desc.CPUAccessFlags;
	texDesc.Format = desc.Format;
	texDesc.Height = desc.Height;
	texDesc.Width = desc.Width;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc = desc.SampleDesc;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	V(g_pd3dDevice->CreateTexture2D(&texDesc, 0, &texture));
	pContext->CopyResource(texture, backbufferRes);

	hr = TTextureMgr::SaveFile(pContext, texture, name);
	texture->Release();
	backbufferRes->Release();
	return hr;
}


bool    TDxRT::Release()
{
	return true;
}
TDxRT::TDxRT()
{
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;
	m_pSRV = nullptr;
	m_pTexture = nullptr;
	m_DSTexFormat = DXGI_FORMAT_R24G8_TYPELESS;
}