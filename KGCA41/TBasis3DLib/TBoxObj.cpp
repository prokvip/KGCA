#include "TBoxObj.h"

bool TBoxObj::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = 5.0f;
	m_matWorld._22 = 5.0f;
	m_matWorld._33 = 5.0f;
	m_matWorld._42 = 40.0f;
	return true;
}
HRESULT TBoxObj::LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pShaderFile)
{
	HRESULT hr = S_OK;
	TActor::LoadShaderFile(pDevice, pShaderFile);

	m_pVSPassThroughMain.Attach(DX::LoadVertexShaderFile(pDevice, pShaderFile, m_pVSBlob.GetAddressOf(), "VS_SO"));
	m_pGS.Attach(DX::LoadGeometryShaderFile(pDevice, pShaderFile, m_pGSBlob.GetAddressOf(), "GS_SO"));

	D3D11_SO_DECLARATION_ENTRY pDecl[] =
	{
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "NORMAL", 0, 0, 3, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 },
		{ 0, "TEXCOORD", 0, 0, 2, 0 },
	};

	int size = ARRAYSIZE(pDecl);
	UINT strides[] = { sizeof(PNCT_VERTEX) };

	if (FAILED(hr = pDevice->CreateGeometryShaderWithStreamOutput(
		m_pGSBlob.Get()->GetBufferPointer(),
		m_pGSBlob.Get()->GetBufferSize(),
		pDecl,
		size,
		strides,
		1,
		D3D11_SO_NO_RASTERIZED_STREAM,// 11.01 이하이면 0을 사용한다. dss=disable
		NULL,
		m_pGSStreamOutput.GetAddressOf())))
	{
		H(hr);
		return hr;
	}

	return S_OK;
}

bool TBoxObj::SetSO(ID3D11Device* pd3dDevice, ID3D11DeviceContext*  pContext)
{
	DX::ApplyDSS(pContext, DX::TDxState::g_pDSSDepthDisable);
	m_RenderSO.CreateStreams(pd3dDevice, sizeof(PNCT_VERTEX), 100000);

	m_RenderSO.BeginRTS(pContext);
	RenderMesh(pContext, true);
	m_RenderSO.EndRTS(pContext);

	for (int iStream = 0; iStream < m_iNumSteamOut; iStream++)
	{
		m_RenderSO.BeginRTS(pContext);
		RenderMesh(pContext, false);
		m_RenderSO.EndRTS(pContext);
	}
	return true;
}
bool TBoxObj::RenderMesh(ID3D11DeviceContext*  pContext, bool bFirstRender)
{
	PreRender(pContext);
	pContext->PSSetShader(nullptr, nullptr, 0);
	pContext->VSSetShader(m_pVSPassThroughMain.Get(), nullptr, 0);
	pContext->GSSetShader(m_pGSStreamOutput.Get(), nullptr, 0);

	if (bFirstRender)
	{
		PostRender(pContext);
	}
	else
	{
		ID3D11Buffer* pVB[1] = { m_RenderSO.m_pDrawFrom.Get() };
		UINT Strides[1] = { sizeof(PNCT_VERTEX) };
		UINT Offsets[1] = { 0 };
		pContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
		pContext->DrawAuto();
		/*
		ID3D11Query* pQuery;
		D3D11_QUERY_DESC queryDesc;
		queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
		queryDesc.MiscFlags = 0;
		m_pd3dDevice->CreateQuery(&queryDesc, &pQuery);

		pContext->Begin(pQuery);
		pContext->DrawAuto();
		pContext->End(pQuery);

		D3D11_QUERY_DATA_PIPELINE_STATISTICS stats;
		while (S_OK != pContext->GetData(pQuery, &stats, pQuery->GetDataSize(), 0));
		SAFE_RELEASE(pQuery);*/
	}
	return true;
}
bool TBoxObj::Render(ID3D11DeviceContext*  pContext)
{
	PreRender(pContext);
	pContext->UpdateSubresource(m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	ID3D11Buffer* pVB[1] = { m_RenderSO.m_pDrawFrom.Get() };
	UINT Strides[1] = { sizeof(PNCT_VERTEX) };
	UINT Offsets[1] = { 0 };
	pContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pContext->GSSetShader(NULL, NULL, 0);
	pContext->DrawAuto();
	return true;
}

TBoxObj::TBoxObj(void)
{
	m_iNumSteamOut = 2;
}

TBoxObj::~TBoxObj(void)
{

}
