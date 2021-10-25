#include "Sample.h"

bool	Sample::PreRender() 
{
	TCore::PreRender();	
	m_pImmediateContext->ClearDepthStencilView(
		m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return true;
}
HRESULT Sample::CreateDepthStencilView()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = Desc.BufferDesc.Width;
	td.Height = Desc.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	svd.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(pTexture, &svd,
		&m_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}
	SAFE_RELEASE(pTexture);

	m_pImmediateContext->OMSetRenderTargets(1,
		&m_pRenderTargetView, m_pDepthStencilView);
	return hr;
}
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
	CreateDepthStencilView();
	CreateDepthStencilState();

	//m_FbxObjA.LoadObject("../../data/object/SM_Tree_Var01.fbx");
	//m_FbxObj.LoadObject("../../data/object/ship.fbx");
	//m_FbxObjA.LoadObject("../../data/object/multiCamera.fbx");
	//m_FbxObjA.LoadObject("../../data/object/BoxAnim.fbx");
	
	CStopwatch stop;
	m_FbxObjA.LoadObject("../../data/object/Turret.fbx", "CharacterShader.hlsl");
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
	m_FbxObjA.Frame();
	m_FbxObjB.Frame();
	return true;
}
bool		Sample::Render() 
{
	m_FbxObjA.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObjA.Render(m_pImmediateContext);	
	m_FbxObjB.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObjB.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release() 
{
	SAFE_RELEASE(m_pDsvState);
	SAFE_RELEASE(m_pDepthStencilView);
	m_FbxObjA.Release();
	m_FbxObjB.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)