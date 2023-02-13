#include "pch.h"
#include "TParticleObj.h"
HRESULT TParticleObj::CreateVertexLayout()
{
	HRESULT hr;
	if (m_pVSCode == nullptr)
	{
		return E_FAIL;
	}
	// 정점레이아웃은 정점쉐이더 밀접한 관련.
	// 정점레이아웃 생성시 사전에 정점쉐이더 생성이 필요하다.
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0,40,D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT NumElements = sizeof(ied) / sizeof(ied[0]);
	hr = m_pd3dDevice->CreateInputLayout(
		ied,
		NumElements,
		m_pVSCode->GetBufferPointer(),
		m_pVSCode->GetBufferSize(),
		&m_pVertexLayout);

	return hr;
}
bool TParticleObj::CreateShader(std::wstring filename)
{
	// vs + ps
	TBaseObject::CreateShader(filename);
	// gs
	DWORD dwShaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined( _DEBUG ) 
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	// 정점쉐이더 컴파일 
	ID3DBlob* pErrorCode = nullptr;
	ID3DBlob* pGSCode = nullptr;
	HRESULT hr = D3DCompileFromFile(
		L"Particle.txt",
		NULL,
		NULL,
		"GS",
		"gs_5_0",
		dwShaderFlags,
		0,
		&pGSCode,
		&pErrorCode);
	if (FAILED(hr))
	{
		if (pErrorCode)
		{
			OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
			pErrorCode->Release();
		}
		return hr;
	}
	hr = m_pd3dDevice->CreateGeometryShader(
		pGSCode->GetBufferPointer(),
		pGSCode->GetBufferSize(),
		NULL,
		&m_pGS);
	return true;
}
bool TParticleObj::Init()
{
	return true;
}
bool TParticleObj::Frame()
{
	for (UINT i = 0; i < m_iParticleCounter; i++)
	{
		m_Paticles[i].fTimer += g_fSecondPerFrame;
		if (m_Paticles[i].fTimer > m_Paticles[i].fLifeTime)
		{
			m_Paticles[i].bEnable = false;
			m_VertexList[i].p = TVector3(0, 0, 0);
			continue;
		}
		m_Paticles[i].vPos = m_Paticles[i].vPos + m_Paticles[i].vVel * g_fSecondPerFrame;
		m_VertexList[i].p = m_Paticles[i].vPos;

		m_VertexList[i].n = m_Paticles[i].vVel;
		m_VertexList[i].c = m_Paticles[i].vColor;
		m_VertexList[i].t = { 0.0f, 0.0f };
	}
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL,
						&m_VertexList.at(0), 0, 0);
	return true;
}
bool TParticleObj::Render()
{
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
	m_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
	m_pImmediateContext->GSSetShader(m_pGS, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
	UINT stride = sizeof(PNCT_VERTEX); // 정점1개의 바이트용량
	UINT offset = 0; // 정점버퍼에서 출발지점(바이트)
	m_pImmediateContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	if (m_pIndexBuffer == nullptr)
		m_pImmediateContext->Draw(m_VertexList.size(), 0);
	else
		m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0);
	return true;
}
bool TParticleObj::Release()
{
	TBaseObject::Release();

	if (m_pGS)m_pGS->Release();
	m_pGS = nullptr;
	return true;
}
void TParticleObj::CreateVertexData()
{
	if (m_VertexList.size() > 0)
	{
		m_InitVertexList = m_VertexList;
		return;
	}
	m_VertexList.resize(m_iParticleCounter);
	for (UINT i = 0; i < m_iParticleCounter; i++)
	{
		m_VertexList[i].p = m_Paticles[i].vPos;
		m_VertexList[i].n = m_Paticles[i].vVel;
		m_VertexList[i].c = m_Paticles[i].vColor;
		m_VertexList[i].t = { 0.0f, 0.0f };
	}
	m_InitVertexList = m_VertexList;
	return;
}
void TParticleObj::CreateIndexData()
{	
	return;
}
void TParticleObj::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
	TBaseObject::SetMatrix(matWorld, matView, matProj);
	m_vPos.x = m_matWorld._41;
	m_vPos.y = m_matWorld._42;
	m_vPos.z = m_matWorld._43;

	m_vRight.x = m_matWorld._11;
	m_vRight.y = m_matWorld._12;
	m_vRight.z = m_matWorld._13;

	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;

}