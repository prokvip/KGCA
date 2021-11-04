#include "TMesh.h"

HRESULT TMesh::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	hr = TModel::CreateConstantBuffer();
	if (m_pVertexList.size() <= 0) return hr;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TAnimMatrix);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_matAnimMatrix;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pAnimCB);
	if (FAILED(hr)) return hr;
	return hr;
}
// 멀티스트림 기술
HRESULT TMesh::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	TModel::CreateVertexBuffer();

	D3D11_BUFFER_DESC bd;	
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCTIW_VERTEX) * m_WeightList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_WeightList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIWVertrexBuffer);
	if (FAILED(hr)) return hr;
	return hr;
}
HRESULT TMesh::CreateVertexLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
		m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr)) return hr;

	m_pVSBlob->Release();
	return hr;
}
bool TMesh::PreRender(ID3D11DeviceContext* pContext)
{
	if (m_pVertexList.size() <= 0) return true;

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->UpdateSubresource(m_pAnimCB, 0, NULL, &m_matAnimMatrix, 0, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->VSSetConstantBuffers(1, 1, &m_pAnimCB);

	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pMainPS, NULL, 0);
	pContext->IASetInputLayout(m_pVertexLayout);

	ID3D11Buffer* vb[2] = { m_pVertexBuffer, m_pIWVertrexBuffer };
	UINT pStrides[2] = {sizeof(PNCT_VERTEX), sizeof(PNCTIW_VERTEX)};
	UINT pOffsets[2] = { 0,0 };
	pContext->IASetVertexBuffers(0, 2, vb,	pStrides, pOffsets);
	pContext->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT, 0);
	return true;
}
bool TMesh::Release() 
{
	TModel::Release();
	for (auto data : m_pSubMesh)
	{
		data->Release();
		SAFE_DEL(data);
	}
	SAFE_RELEASE(m_pIWVertrexBuffer);
	SAFE_RELEASE(m_pAnimCB);
	return true;
}
TMesh::TMesh()
{
	m_ClassType = CLASS_GEOM;
	m_iMtrlRef = -1;
}
TMesh::~TMesh()
{
}