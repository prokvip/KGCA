#include "pch.h"
#include "Sample.h"
const float g_fMaxSize = 4096;
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
void Sample::InitRT()
{
	m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f, -0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);
	
	m_pShadowConstantBuffer.Attach(CreateConstantBuffer(
		m_pd3dDevice.Get(), &m_cbShadow, 1, sizeof(SHADOW_CONSTANT_BUFFER)));


	m_RT.m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
	m_RT.Create(m_pd3dDevice.Get(), g_fMaxSize, g_fMaxSize);// m_bColorTexRender );	
	D3DXMatrixOrthoOffCenterLH(&m_matShadowProj,
		-100 / 2, 100 / 2, -100 / 2, 100 / 2, 0.0f, 500.0f);

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
	if (FAILED(hr = m_pd3dDevice->CreateSamplerState(&SamDescShad, 
		g_pSSShadowMap.GetAddressOf())))
	{
		return;
	}
}
void Sample::PreDepthShadow()
{
	TVector4 vClearColor(1.0f, 1.0f, 1.0f, 1.0f);	
	
	if (m_RT.Begin(m_pImmediateContext.Get(), vClearColor))
	{		
		ObjectShadow();
		m_RT.End(m_pImmediateContext.Get());
	}
}
bool Sample::ObjectShadow()
{
	if (m_pCurrentScene == nullptr) return true;

	m_pImmediateContext->RSSetState(TDxState::g_pRSSlopeScaledDepthBias);


	TSceneTitle* pScene = (TSceneTitle*)m_pCurrentScene.get();
	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_pImmediateContext->OMSetDepthStencilState(
		TDxState::g_pDefaultDepthStencil, 0xff);
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pImmediateContext->GSSetShader(nullptr, NULL, 0);

	TVector3 vSunLightDir(0, 100, 100);
	D3DXVec3Normalize(&vSunLightDir, &-vSunLightDir);
	TVector3 vLightPos(0, 50, -50);
	TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);
	//vLight = vLight * matRotation;
	D3DXVec3TransformCoord(&vLightPos, &vLightPos, &matRotation);
	TVector3 vLightDir;
	D3DXVec3Normalize(&vLightDir, &-vLightPos);	

	TVector3 vLookat = TVector3(0, 0, 0);
	TVector3 vUp = TVector3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matShadowView, &vLightPos, &vLookat, &vUp);


	TMatrix matWVPT = m_matShadowView * m_matShadowProj * m_matTexture;
	D3DXMatrixTranspose(&m_cbShadow.g_matShadow, &matWVPT);


	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil, 0xff);

	if (pScene->m_pMap)
	{
		pScene->m_pMap->m_cbData.vLightDir =
			TVector4(vLightDir.x, vLightDir.y, vLightDir.z, 100.0f);
		pScene->m_pMap->m_cbData.vLightPos =
			TVector4(vLightPos.x, vLightPos.y, vLightPos.z, 55.0f);
		pScene->m_pMap->m_cbData.vEyeDir =
		{
			pScene->m_pMainCamera->m_vLook.x,
			pScene->m_pMainCamera->m_vLook.y,
			pScene->m_pMainCamera->m_vLook.z,
			0.90f
		};
		pScene->m_pMap->m_cbData.vEyePos = {
			pScene->m_pMainCamera->m_vPos.x,
			pScene->m_pMainCamera->m_vPos.y,
			pScene->m_pMainCamera->m_vPos.z,
			0.98f
		};
		pScene->m_pMap->SetMatrix(nullptr,
			&m_matShadowView,
			&m_matShadowProj);
		m_Quadtree.Frame();
		m_Quadtree.m_pMap->m_pImmediateContext->PSSetShaderResources(
			6, 1, m_RT.m_pDsvSRV.GetAddressOf());

		m_Quadtree.Render();
	}

	TMatrix matWorld;
	if (pScene->m_pUser)
	{
		matWorld._41 = pScene->m_pUser->m_vPos.x;
		matWorld._42 = pScene->m_pUser->m_vPos.y;
		matWorld._43 = pScene->m_pUser->m_vPos.z;
	}
	if (m_UserCharacter)
	{
		m_UserCharacter->SetMatrix(&matWorld, 
			&m_matShadowView,
			&m_matShadowProj);
		m_UserCharacter->Render();
	}
	return true;
}