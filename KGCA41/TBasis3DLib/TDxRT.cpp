#include "TDxRT.h"
namespace DX
{
	TDxRT::TDxRT()
	{
		m_pRenderTargetView = nullptr;
		m_pDepthStencilView = nullptr;
		m_pSRV = nullptr;
		m_pTexture = nullptr;
		m_DSFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}
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
	HRESULT TDxRT::CreateCubeMap(ID3D11Device*	pd3dDevice, FLOAT fWidth, FLOAT fHeight)
	{
		HRESULT hr;
		Set(pd3dDevice, 0, 0, fWidth, fHeight, 0.0f, 1.0f);

		// ����Ÿ�Ͽ� �ؽ�ó ����
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

		// ����Ÿ�Ͽ� �� ����
		D3D11_RENDER_TARGET_VIEW_DESC RtvDesc;
		ZeroMemory(&RtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		RtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		RtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		RtvDesc.Texture2DArray.ArraySize = 6;
		V(pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), &RtvDesc, m_pRenderTargetView.GetAddressOf()));

		// ���̴� ���ҽ� �� ����
		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
		ZeroMemory(&SrvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SrvDesc.Format = TexDesc.Format;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SrvDesc.Texture2DArray.MipLevels = 1;
		V(pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &SrvDesc, m_pSRV.GetAddressOf()));


		// ���� ���ٽ� �� �ؽ�ó ����
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

		// ���� ���ٽ� �� ����
		D3D11_DEPTH_STENCIL_VIEW_DESC DsvDesc;
		ZeroMemory(&DsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		DsvDesc.Format = DSDesc.Format;
		DsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		DsvDesc.Texture2DArray.ArraySize = 6;
		V(pd3dDevice->CreateDepthStencilView(pDSTexture.Get(), &DsvDesc, &m_pDepthStencilView));
		return S_OK;
	}
	HRESULT TDxRT::Create(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height)
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


		// ������� ũ�Ⱑ ����Ǹ� �ݵ�� ���̽��ٽ� �� �ٽ� �ۼ��Ǿ�� �Ѵ�.		
		if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height)))
		{
			return hr;
		}
		return hr;
	}
	HRESULT TDxRT::UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height)
	{
		HRESULT hr;
		m_vp.Width = (FLOAT)Width;
		m_vp.Height = (FLOAT)Height;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
		D3D11_TEXTURE2D_DESC DescDepth;
		ZeroMemory(&DescDepth, sizeof(D3D11_TEXTURE2D_DESC));
		DescDepth.Width = Width;
		DescDepth.Height = Height;
		DescDepth.MipLevels = 1;
		DescDepth.ArraySize = 1;
		if(m_DSFormat == DXGI_FORMAT_D24_UNORM_S8_UINT)
			DescDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
		else
			DescDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		DescDepth.SampleDesc.Count = 1;
		DescDepth.SampleDesc.Quality = 0;		
		DescDepth.CPUAccessFlags = 0;
		DescDepth.MiscFlags = 0;
		
		DescDepth.Usage = D3D11_USAGE_DEFAULT;
		// �� ���� ���� �� ���ٽ� ���� ����
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		if (DescDepth.Format == DXGI_FORMAT_R24G8_TYPELESS || DescDepth.Format == DXGI_FORMAT_R32_TYPELESS)
		{
			// ���̸� ���� ���̸� ����
			DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		}


		/*DXGI_FORMAT texturefmt = DXGI_FORMAT_R32_TYPELESS;
		if(m_DSFormat == DXGI_FORMAT_D24_UNORM_S8_UINT)
			texturefmt = DXGI_FORMAT_R24G8_TYPELESS;
		else
			texturefmt = DXGI_FORMAT_R32_TYPELESS;*/

		//D3D11_TEXTURE2D_DESC dtd =
		//{
		//	Width,
		//	Height,
		//	1,//UINT MipLevels;
		//	1,//UINT ArraySize;
		//	texturefmt,//DXGI_FORMAT Format;
		//	1,//DXGI_SAMPLE_DESC SampleDesc;
		//	0,
		//	D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		//	D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		//	0,//UINT CPUAccessFlags;
		//	0//UINT MiscFlags;    
		//};
		if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, pDSTexture.GetAddressOf())))
		{
			return hr;
		}

		///// ���̴� ���ҽ� ���� : ���� �� �����쿡�� ����Ѵ�. ///
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
		

		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		if (FAILED(hr = pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, m_pDepthStencilView.ReleaseAndGetAddressOf())))
		{
			return hr;
		}	
		m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);

		/*D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd =
		{
			srvDesc.Format,
			D3D11_SRV_DIMENSION_TEXTURE2D,
			0,
			0
		};
		dsrvd.Texture2D.MipLevels = 1;*/

		if (srvDesc.Format == DXGI_FORMAT_R32_FLOAT || srvDesc.Format == DXGI_FORMAT_R24_UNORM_X8_TYPELESS)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			if (FAILED(hr = pDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, m_pDsvSRV.ReleaseAndGetAddressOf())))
			{
				return hr;
			}
		}
		return hr;
	}
	void TDxRT::Apply(ID3D11DeviceContext*    pImmediateContext,
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
	bool	TDxRT::Begin(ID3D11DeviceContext*    pContext, TVector4 vColor,
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
	bool	TDxRT::Clear(ID3D11DeviceContext*    pContext, TVector4 vColor,
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
	bool	TDxRT::End(ID3D11DeviceContext*    pContext)
	{
		pContext->RSSetViewports(m_nViewPorts, m_vpOld);
		pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
		SAFE_RELEASE(m_pOldRTV);
		SAFE_RELEASE(m_pOldDSV);
		return true;
	}
	HRESULT TDxRT::SaveFile(ID3D11DeviceContext*    pContext, T_STR name)
	{
		HRESULT hr=S_OK;
		// ����Ÿ�Ϻ信�� �ؽ�ó ���Ϸ� ����	
		if (m_pTexture != nullptr)
		{
			if (TTextureMgr::SaveFile(pContext, m_pTexture.Get(), name))
			{
				return hr;
			}
		}
		else if (m_pSRV != nullptr)
		{
			ID3D11Resource*         resource;
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
			hr = TTextureMgr::SaveFile(pContext, texture.Get(), name+L"RTV");
		}
		return hr;
	}

	//--------------------------------------------------------------------------------------
	// ����� ���� 
	//--------------------------------------------------------------------------------------
	HRESULT TDxRT::SaveBackBuffer(ID3D11DeviceContext*    pContext, T_STR name)
	{			
		HRESULT hr = S_OK;
		ID3D11Resource *backbufferRes;
		m_pRenderTargetView.Get()->GetResource(&backbufferRes);
		

		ID3D11Texture2D* tex=nullptr;
		if (FAILED(hr = backbufferRes->QueryInterface(__uuidof(ID3D11Texture2D),
			reinterpret_cast<LPVOID*>(&tex))))
		{

		}
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);
		backbufferRes->Release();


		ID3D11Texture2D *texture;
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
}