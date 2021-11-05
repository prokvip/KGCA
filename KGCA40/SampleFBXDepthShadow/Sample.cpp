#include "Sample.h"
HRESULT Sample::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(cbDataShadow);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_ShadowCB;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pShadowCB);
	if (FAILED(hr)) return hr;
	return hr;
}
bool		Sample::Init()
{	
	CreateConstantBuffer();
	m_Camera.CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 500.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);


	m_matTex._11 = 0.5f; m_matTex._22 = -0.5f;
	m_matTex._41 = 0.5f; m_matTex._42 = 0.5f;
	m_Light1.Set(TVector3(150, 150, 150), TVector3(0, 0, 0));
	
	m_MiniMap.Create(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt",L"");
	m_MapObj.Create(L"ProjectionShadow.txt",
					L"ProjectionShadow.txt",L"../../data/kgcabk.bmp");
	TMatrix matWorld, matScale;
	D3DXMatrixRotationX(&matWorld, XM_PI / 2.0f);
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	m_MapObj.m_matWorld= matScale * matWorld;
	m_Rt.Create(1024, 1024);	

	ID3DBlob* PSBlob = nullptr;	
	PSBlob = TModel::LoadShaderBlob(L"CharacterShader.hlsl","PSShadow", "ps_5_0");
	if (PSBlob != nullptr)
	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(
			PSBlob->GetBufferPointer(),
			PSBlob->GetBufferSize(),
			NULL, &m_pPSShadow);
		if (FAILED(hr)) return hr;
		PSBlob->Release();
	}
	PSBlob = TModel::LoadShaderBlob(L"ProjectionShadow.txt",
		"PSDepth", "ps_5_0");
	if (PSBlob != nullptr)
	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(
			PSBlob->GetBufferPointer(),
			PSBlob->GetBufferSize(),
			NULL, &m_pPSShadowMap);
		if (FAILED(hr)) return hr;
		PSBlob->Release();
	}
	m_FbxCharacter.LoadObject("../../data/object/Man.fbx", "CharacterShader.hlsl");
	

	return true;
}
bool		Sample::Frame() 
{
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_FbxCharacter.m_bAnimPlay = !m_FbxCharacter.m_bAnimPlay;
	}
	m_FbxCharacter.Frame();
	m_Light1.Frame();	
	
	m_ShadowCB.g_matShadow1 =m_Light1.m_matView * m_Light1.m_matProj *m_matTex;
	return true;
}
bool		Sample::Render() 
{	
	ApplyRS(m_pImmediateContext, TDxState::g_pRSDepthBias);
	
	if (m_Rt.Begin(m_pImmediateContext))
	{
		m_MapObj.SetMatrix(&m_MapObj.m_matWorld, 
			&m_Light1.m_matView, &m_Light1.m_matProj);		
		m_MapObj.PreRender(m_pImmediateContext);
		m_pImmediateContext->PSSetShader(m_pPSShadowMap, NULL, 0);
		m_MapObj.PostRender(m_pImmediateContext,
			m_MapObj.m_iNumIndex);//

		m_FbxCharacter.SetMatrix(&m_FbxCharacter.m_matWorld, 
			&m_Light1.m_matView, &m_Light1.m_matProj);
		m_FbxCharacter.SetPixelShader(m_pPSShadow);
		m_FbxCharacter.Render(m_pImmediateContext);
		m_Rt.End(m_pImmediateContext);
	}

	m_ShadowCB.g_matShadow1 = m_ShadowCB.g_matShadow1.Transpose();
	m_pImmediateContext->UpdateSubresource(
		m_pShadowCB, 0, NULL, &m_ShadowCB, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pShadowCB);

	ApplyRS(m_pImmediateContext, TDxState::g_pRSSolid);
	ApplySS(m_pImmediateContext, TDxState::g_pClampSS,1);
	
	D3DXMatrixInverse(&m_MapObj.m_cbData.matNormal, NULL, 
		&m_MapObj.m_matWorld);
	m_MapObj.m_cbData.vLightDir.x = m_Light1.m_vDir.x;
	m_MapObj.m_cbData.vLightDir.y = m_Light1.m_vDir.y;
	m_MapObj.m_cbData.vLightDir.z = m_Light1.m_vDir.z;
	m_MapObj.SetMatrix(&m_MapObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Rt.m_pTextureSRV);
	m_MapObj.Render(m_pImmediateContext);

	m_MiniMap.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_Rt.m_pTextureSRV);
	m_MiniMap.PostRender(m_pImmediateContext, m_MiniMap.m_iNumIndex);

	D3DXMatrixInverse(&m_FbxCharacter.m_cbData.matNormal, 
		NULL, &m_FbxCharacter.m_matWorld);
	m_FbxCharacter.SetMatrix(&m_FbxCharacter.m_matWorld, 	&m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxCharacter.SetPixelShader(nullptr, 
		&m_FbxCharacter.m_cbData.matNormal, m_Light1.m_vDir);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Rt.m_pTextureSRV);
	m_FbxCharacter.Render(m_pImmediateContext);

	if (g_Input.GetKey(VK_F5) == KEY_PUSH)
	{
		m_Rt.Save(m_pImmediateContext,L"frame.dds");
	}
	return true;
}
bool		Sample::Release() 
{	
	m_MapObj.Release();
	m_MiniMap.Release();
	m_Rt.Release();
	m_FbxCharacter.Release();
	SAFE_RELEASE(m_pPSShadow);
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)