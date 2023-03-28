#include "TMiniMap.h"
void TMiniMap::Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
	FLOAT MinDepth, FLOAT MaxDepth)
{
	m_vp.TopLeftX = TopLeftX;
	m_vp.TopLeftY = TopLeftY;
	m_vp.Width = Width;
	m_vp.Height = Height;
	m_vp.MinDepth = MinDepth;
	m_vp.MaxDepth = MaxDepth;
}
bool TMiniMap::Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTexture)
{
	m_pPlaneShape = make_shared<TPlaneShape>();
	if (FAILED(m_pPlaneShape->Create(pd3dDevice, pLoadShaderFile)))
	{
		MessageBox(0, _T("m_LineShape ½ÇÆÐ"), _T("Fatal error"), MB_OK);
		return 0;
	}

	m_pMiniMapRT = make_shared<TDxRT>();
	if (FAILED(m_pMiniMapRT->Create(pd3dDevice)))
	{
		return false;
	}

	DWORD dwWidth = m_pMiniMapRT->m_TexDesc.Width;
	DWORD dwHeight = m_pMiniMapRT->m_TexDesc.Height;
	TVector3 vUpVector(0.0f, 1.0f, 0.0f);
	SetViewMatrix(TVector3(0.0f, 70.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f), vUpVector);
	SetProjMatrix(XM_PI * 0.25f, (float)dwWidth / (float)dwHeight, 0.1f, 1000.0f);

	return true;
}

bool TMiniMap::BeginRender(ID3D11DeviceContext*    pContext, TVector4 vColor )
{
	if (m_pMiniMapRT->Begin(pContext, vColor) == false)
	{
		m_pMiniMapRT->End(pContext);
		return false;
	}
	return true;
}
bool TMiniMap::EndRender(ID3D11DeviceContext*    pContext)
{
	m_pMiniMapRT->End(pContext);
	return true;
}
bool TMiniMap::Frame()
{
	return true;
}
bool TMiniMap::Render(ID3D11DeviceContext*    pContext)
{
	m_pPlaneShape->m_dxobj.g_pTextureSRV = m_pMiniMapRT->m_pSRV;
	pContext->RSSetViewports(1, &m_vp);
	m_pPlaneShape->SetMatrix(NULL, NULL, NULL);
	pContext->UpdateSubresource(m_pPlaneShape->m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_pPlaneShape->m_cbData, 0, 0);
	m_pPlaneShape->Render(pContext);
	return true;
}
HRESULT TMiniMap::CreateResource(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pImmediateContext, UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;
	return hr;
}
HRESULT TMiniMap::DeleteResource()
{
	HRESULT hr = S_OK;
	return S_OK;
}

bool TMiniMap::Release()
{
	return true;
}
TMiniMap::TMiniMap(void)
{
	m_pPlaneShape = nullptr;
}

TMiniMap::~TMiniMap(void)
{
	Release();
}
