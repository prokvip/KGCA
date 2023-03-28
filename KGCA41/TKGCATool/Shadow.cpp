#include "pch.h"
#include "Sample.h"
const float g_fMaxSize = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
ID3D11Buffer* Sample::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex,
	UINT iSize, bool bDynamic)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	if (bDynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iNumIndex;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	InitData.pSysMem = data;
	if (data != NULL)
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
		{
			H(hr);
			return nullptr;
		}
	}
	else
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
		{
			H(hr);
			return nullptr;
		}
	}
	return pBuffer;
}
void Sample::SetShadowProjectionDistance()
{	
	m_pShadowCamera->CreateOrthoProjMatrix(1000,1000, m_fDepthMapCameraNear, m_fDepthMapCameraFar);
}
void Sample::InitRT()
{
	m_SlopeScaledDepthBiasDesc = TDxState::g_RasterizerDesc;
	m_SlopeScaledDepthBiasDesc.SlopeScaledDepthBias = 10.0f;
	TDxState::SetRasterizerState(m_pd3dDevice.Get(), m_pImmediateContext.Get(), m_SlopeScaledDepthBiasDesc);

	m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f, -0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);
	
	m_pShadowConstantBuffer.Attach(CreateConstantBuffer(
		m_pd3dDevice.Get(), &m_cbShadow, 1, sizeof(SHADOW_CONSTANT_BUFFER)));


	m_RT.m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
	m_RT.Create(m_pd3dDevice.Get(), g_fMaxSize, g_fMaxSize);// m_bColorTexRender );	
	

	HRESULT hr;
	D3D11_SAMPLER_DESC SamDescShad =
	{
		D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,// D3D11_FILTER Filter;
		D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressU;
		D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressV;
		D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressW;
		0,//FLOAT MipLODBias;
		0,//UINT MaxAnisotropy;
		D3D11_COMPARISON_LESS , //D3D11_COMPARISON_FUNC ComparisonFunc;
		0.0,0.0,0.0,0.0,//FLOAT BorderColor[ 4 ];
		0,//FLOAT MinLOD;
		0//FLOAT MaxLOD;   
	};
	if (FAILED(hr = m_pd3dDevice->CreateSamplerState(&SamDescShad, g_pSSShadowMap.GetAddressOf())))
	{
		return;
	}
}
void Sample::PreDepthShadow(ID3D11DeviceContext* pContext)
{
	TVector4 vClearColor(1.0f, 1.0f, 1.0f, 1.0f);	
	
	if (m_RT.Begin(pContext, vClearColor))
	{		
		TDxState::SetRasterizerState(m_pd3dDevice.Get(), pContext, m_SlopeScaledDepthBiasDesc);
		ApplyRS(pContext, TDxState::g_pRSEdit.Get());
		ObjectShadow(pContext);
		m_RT.End(pContext);
	}
}
bool Sample::ObjectShadow(ID3D11DeviceContext* pContext)
{
	if (m_pCurrentScene == nullptr) return true;

	TSceneTitle* pScene = (TSceneTitle*)m_pCurrentScene.get();
	pContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	pContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil, 0xff);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->GSSetShader(nullptr, NULL, 0);

	TVector3 vSunLightDir(0, 100, 100);
	D3DXVec3Normalize(&vSunLightDir, &-vSunLightDir);
	TVector3 vLightPos(0, 50, 50);
	TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);	
	D3DXVec3TransformCoord(&vLightPos, &vLightPos, &matRotation);
	
	TVector3 vLookat = TVector3(0, 0, 0);
	TVector3 vUp = TVector3(0.0f, 1.0f, 0.0f);
	m_pShadowCamera->CreateViewMatrix(vLightPos, vLookat, vUp);	
	m_matShadowView = m_pShadowCamera->m_matView;
	m_matShadowProj = m_pShadowCamera->m_matProj;
	//D3DXMatrixLookAtLH(&m_matShadowView, &vLightPos, &vLookat, &vUp);
	TMatrix matWVPT = m_matShadowView * m_matShadowProj * m_matTexture;
	D3DXMatrixTranspose(&m_cbShadow.g_matShadow, &matWVPT);

	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil, 0xff);

	if (pScene->m_pMap)
	{
		pScene->m_pMap->SetMatrix(nullptr,	&m_matShadowView,	&m_matShadowProj);		
		m_Quadtree.RenderShadow(pContext, m_pCurrentScene->m_pMainCamera);
	}
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TMatrix matWorld;
	if (pScene->m_pUser)
	{
		matWorld._41 = pScene->m_pUser->m_vPos.x;
		matWorld._42 = pScene->m_pUser->m_vPos.y;
		matWorld._43 = pScene->m_pUser->m_vPos.z;
	}
	if (m_UserCharacter)
	{
		m_UserCharacter->SetMatrix(&matWorld, &m_matShadowView,	&m_matShadowProj);
		m_UserCharacter->PreRender(pContext);
		pContext->PSSetShader(NULL, NULL, 0);
		m_UserCharacter->PostRender(pContext);
	}

	ClearD3D11DeviceContext(pContext);
	return true;
}