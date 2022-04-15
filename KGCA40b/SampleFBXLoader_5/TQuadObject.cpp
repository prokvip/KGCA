#include "TQuadObject.h"
#include "TInput.h"
HRESULT TQuadObject::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr;
	// Create a constant buffer
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VSCB);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &m_MaskValue;

	hr = pd3dDevice->CreateBuffer(&cbDesc, NULL, m_pMaskCB.GetAddressOf());
	return hr;
}
bool TQuadObject::SetBuffer(ID3D11Device* pd3dDevice)
{
	if (FAILED(CreateConstantBuffer(pd3dDevice)))
	{
		MessageBox(0, _T("CreateConstantBuffer  ½ÇÆÐ"), _T("Fatal error"), MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_MASKCOUNT * MAX_MASKCOUNT * sizeof(float),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0
	};
	pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pMaskBuffer.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	// Again, we need a resource view to use it in the shader
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.ElementOffset = 0;
	SRVDesc.Buffer.ElementWidth = MAX_MASKCOUNT * MAX_MASKCOUNT;
	pd3dDevice->CreateShaderResourceView(m_pMaskBuffer.Get(), &SRVDesc, &m_pMaskArray);
	return true;
}

void TQuadObject::ComputeKernel(int iRadius )
{	
	float sigma = 1.0f;
	m_iMask = iRadius * 2 + 1;	
	m_pfKernelMask = new float[m_iMask*m_iMask];
	float mean =m_iMask/2.0f;
	float sum = 0.0f; 

	for (int x = 0; x < m_iMask; ++x) 
	{
		for (int y = 0; y < m_iMask; ++y) 
		{
			m_pfKernelMask[x*m_iMask+y] = expf( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
							 / (2 * 3.141592f * sigma * sigma);

			sum += m_pfKernelMask[x*m_iMask+y];
		}
	}
	for (int x = 0; x < m_iMask; ++x) 
	{
		for (int y = 0; y < m_iMask; ++y)
		{
			m_pfKernelMask[x*m_iMask+y] /= sum;
		}
	}
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TQuadObject::CreateTextures(ID3D11Device* pDevice, UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;
	// Create the offscreen buffer
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = iWidth;
	texDesc.Height = iHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	V_RETURN(pDevice->CreateTexture2D(&texDesc, NULL, &m_pColorTex));
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	V_RETURN(pDevice->CreateTexture2D(&texDesc, NULL, &m_pNormalDepthTex));

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	V_RETURN(pDevice->CreateRenderTargetView(m_pColorTex.Get(), &RTVDesc,	&m_pColorRTV));
	RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	V_RETURN(pDevice->CreateRenderTargetView(m_pNormalDepthTex.Get(), &RTVDesc, &m_pNormalDepthRTV));

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;

	V_RETURN(pDevice->CreateShaderResourceView(m_pColorTex.Get(), &SRVDesc, &m_pColorSRV));
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	V_RETURN(pDevice->CreateShaderResourceView(m_pNormalDepthTex.Get(), &SRVDesc, &m_pNormalDepthSRV));


	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = iWidth;
	DescDepth.Height = iHeight;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	V_RETURN(pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DescDepth.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	V_RETURN(pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvd, &m_pDepthStencilView));
	return S_OK;
}
bool TQuadObject::Frame()
{
	if(TInput::Get().GetKey('Z'))
	{
		m_MaskValue.g_fMask += 10.0f*g_fSecPerFrame;
		if (m_MaskValue.g_fMask > 30.0f)
			m_MaskValue.g_fMask = 30;
	}
	if (TInput::Get().GetKey('X'))
	{
		m_MaskValue.g_fMask -= 10.0f*g_fSecPerFrame;
		if (m_MaskValue.g_fMask < 3.0f)
			m_MaskValue.g_fMask = 3;
	}
	return true;
}
bool TQuadObject::Render()
{
	PreRender();
	Draw();

	m_pContext->PSSetShaderResources(0, 1, m_pColorSRV.GetAddressOf());
	m_pContext->PSSetShaderResources(1, 1, m_pNormalDepthSRV.GetAddressOf());

	D3D11_BOX		m_BoxVB; 
	m_BoxVB.left = 0;
	m_BoxVB.right = m_iMask*m_iMask*sizeof(float);
	m_BoxVB.top = 0; m_BoxVB.bottom = 1;
	m_BoxVB.front = 0; m_BoxVB.back = 1;
	m_pContext->UpdateSubresource(m_pMaskBuffer.Get(), 0, &m_BoxVB, m_pfKernelMask, 0, 0);
	
	ID3D11ShaderResourceView* aRViews[1] = { m_pMaskArray.Get() };
	m_pContext->PSSetShaderResources(2, 1, aRViews);

	m_pContext->UpdateSubresource(m_pMaskCB.Get(), 0, NULL, &m_MaskValue, 0, 0);
	m_pContext->PSSetConstantBuffers(1, 1, m_pMaskCB.GetAddressOf());

	PostRender();
	return true;
}
TQuadObject::TQuadObject(void)
{
	m_pfKernelMask = 0;	
	m_iMask = 3;
	m_MaskValue.g_fMask = m_iMask;
	m_MaskValue.g_fTexSizeX = 800.0f;
	m_MaskValue.g_fTexSizeY = 600.0f;
}


TQuadObject::~TQuadObject(void)
{	
	SAFE_DELETE_ARRAY(m_pfKernelMask);
}
