#include "Sample.h"

HRESULT Sample::CreateDepthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	sd.DepthEnable = TRUE;
	sd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_pd3dDevice->CreateDepthStencilState(&sd, &m_pDsvState);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pImmediateContext->OMSetDepthStencilState(m_pDsvState, 0x01);
	return hr;
}
bool		Sample::Init()
{	
	m_MiniMap.Create(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt",L"");
	m_MapObj.Create(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt",L"../../data/kgcabk.bmp");
	TMatrix matWorld, matScale;
	D3DXMatrixRotationX(&matWorld, XM_PI / 2.0f);
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	m_MapObj.m_matWorld= matScale * matWorld;
	m_Rt.Create(4096, 4096);
	TPlane p;
	p.x = 0.0f;
	p.y = 1.0f;
	p.z = 0.0f;
	p.w = -0.1f;
	TVector4 v;
	v.x = 100.0f;
	v.y = 100.0f;
	v.z = 100.0f;
	v.w = 0.0f;
	D3DXMatrixShadow(&m_matShadow, &v, &p);

	ID3DBlob* PSBlob = nullptr;	
	PSBlob = TModel::LoadShaderBlob(L"CharacterShader.hlsl",
		"PSShadow", "ps_5_0");
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
		
	CreateDepthStencilState();
	
	//m_FbxObjA.LoadObject("../../data/object/SM_Tree_Var01.fbx");
	//m_FbxObj.LoadObject("../../data/object/ship.fbx");
	//m_FbxObjA.LoadObject("../../data/object/multiCamera.fbx");
	//m_FbxObjA.LoadObject("../../data/object/BoxAnim.fbx");
	
	CStopwatch stop;
	//m_FbxObjA.LoadObject("../../data/object/Turret.fbx", "CharacterShader.hlsl");
	m_FbxObjB.LoadObject("../../data/object/Man.fbx", "CharacterShader.hlsl");
	stop.Output(L"a");
	m_Camera.CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool		Sample::Frame() 
{
	if (g_Input.GetKey(VK_F3) == KEY_PUSH)
	{
		m_FbxObjA.m_bAnimPlay = !m_FbxObjA.m_bAnimPlay;
	}
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_FbxObjB.m_bAnimPlay = !m_FbxObjB.m_bAnimPlay;
	}
	for (int iObj = 0; iObj < m_pObjectList.size(); iObj++)
	{
		m_pObjectList[iObj]->Frame();
	}
	//m_FbxObjA.Frame();
	//m_FbxObjB.Frame();

	
	if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
	{
		m_vMoePos.z += g_fSecPerFrame * 100.0f;
	}
	if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
	{
		m_vMoePos.z -= g_fSecPerFrame * 100.0f;
	}
	if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
	{
		D3DXMatrixRotationY(&m_FbxObjB.m_matWorld, g_fGameTimer);		
	}
	m_FbxObjB.m_matWorld._41 = m_vMoePos.x;
	m_FbxObjB.m_matWorld._42 = m_vMoePos.y;
	m_FbxObjB.m_matWorld._43 = m_vMoePos.z;
	return true;
}
bool		Sample::Render() 
{	
	m_pImmediateContext->RSSetState(m_pRSSolid);
	
	if (m_Rt.Begin(m_pImmediateContext))
	{
		m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, 
			&m_Camera.m_matView, &m_Camera.m_matProj);
		m_FbxObjB.SetPixelShader(m_pPSShadow);
		m_FbxObjB.Render(m_pImmediateContext);
		m_Rt.End(m_pImmediateContext);
	}
	
	m_MapObj.SetMatrix(&m_MapObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_MapObj.Render(m_pImmediateContext);

	m_MiniMap.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(
		0, 1, &m_Rt.m_pTextureSRV);
	m_MiniMap.PostRender(m_pImmediateContext, 
		m_MiniMap.m_iNumIndex);

	m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObjB.SetPixelShader(nullptr);
	m_FbxObjB.Render(m_pImmediateContext);

	if (g_Input.GetKey(VK_F5) == KEY_PUSH)
	{
		m_Rt.Save(m_pImmediateContext,L"frame.jpg");
	}
	return true;
}
bool		Sample::Release() 
{	
	m_MapObj.Release();
	m_MiniMap.Release();
	m_Rt.Release();
	SAFE_RELEASE(m_pDsvState);	
	//FbxObjA.Release();
	m_FbxObjB.Release();
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