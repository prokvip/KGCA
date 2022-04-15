#include "Sample.h"
#include "TObjectMgr.h"
void ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (pd3dDeviceContext == NULL) return;

	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV = NULL;
	ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	// Shaders
	pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->HSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->DSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	// IA clear
	pd3dDeviceContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
	pd3dDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	pd3dDeviceContext->IASetInputLayout(NULL);

	// Constant buffers
	pd3dDeviceContext->VSSetConstantBuffers(0, 14, pBuffers);
	pd3dDeviceContext->HSSetConstantBuffers(0, 14, pBuffers);
	pd3dDeviceContext->DSSetConstantBuffers(0, 14, pBuffers);
	pd3dDeviceContext->GSSetConstantBuffers(0, 14, pBuffers);
	pd3dDeviceContext->PSSetConstantBuffers(0, 14, pBuffers);

	// Resources
	pd3dDeviceContext->VSSetShaderResources(0, 16, pSRVs);
	pd3dDeviceContext->HSSetShaderResources(0, 16, pSRVs);
	pd3dDeviceContext->DSSetShaderResources(0, 16, pSRVs);
	pd3dDeviceContext->GSSetShaderResources(0, 16, pSRVs);
	pd3dDeviceContext->PSSetShaderResources(0, 16, pSRVs);

	// Samplers
	pd3dDeviceContext->VSSetSamplers(0, 16, pSamplers);
	pd3dDeviceContext->HSSetSamplers(0, 16, pSamplers);
	pd3dDeviceContext->DSSetSamplers(0, 16, pSamplers);
	pd3dDeviceContext->GSSetSamplers(0, 16, pSamplers);
	pd3dDeviceContext->PSSetSamplers(0, 16, pSamplers);

	// Render targets
	pd3dDeviceContext->OMSetRenderTargets(8, pRTVs, pDSV);

	// States
	FLOAT blendFactor[4] = { 0,0,0,0 };
	pd3dDeviceContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
	pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	pd3dDeviceContext->RSSetState(NULL);
}
void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
bool	Sample::LoadMap()
{
	m_MapObj.Init();
	m_MapObj.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_MapObj.CreateHeightMap(L"../../data/map/heightMap513.bmp");
	m_MapObj.CreateMap(m_MapObj.m_iNumCols, m_MapObj.m_iNumRows, 10.0f);
	if (!m_MapObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),
		L"MapRT.hlsl",
		L"../../data/map/002.jpg"))
	{
		return false;
	}
	m_Quadtree.m_pCamera = m_pMainCamera;
	m_Quadtree.Build(&m_MapObj, 5);
	/*CreateMapObject();
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		m_Quadtree.AddObject(m_pObjList[iObj]);
	}*/
	return true;
}
bool    Sample::LoadFbx()
{
	std::vector<std::wstring> listname;
	// Greystone.fbx  LOD 메쉬 5개 
	listname.push_back(L"../../data/fbx/Greystone.fbx");
	listname.push_back(L"../../data/fbx/idle.fbx");
	listname.push_back(L"../../data/fbx/Man.fbx");
	// 0 ~ 60  idel
	// 61 ~91  walk;
	// 92 ~ 116	  run
	// 120 ~205 jump
	// 	205 ~289  attack
	//listname.push_back(L"../../data/fbx/ManSkin.fbx");
	//listname.push_back(L"../../data/fbx/ManIdelIdel.fbx");
	//listname.push_back(L"../../data/fbx/ManIdelWalk.fbx");
	//listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/SM_Barrel.fbx");	
	//listname.push_back(L"../../data/fbx/st00sc00.fbx");
	//listname.push_back(L"../../data/fbx/SM_Tree_Var01.fbx");
	//listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");
	listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");

	I_ObjectMgr.Set(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_FbxObj.resize(listname.size());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbx* pFbx = &m_FbxObj[iObj];
		pFbx->Init();
		pFbx->m_pMainCamera = m_pMainCamera;
		pFbx->m_pd3dDevice = m_pd3dDevice.Get();
		pFbx->m_pContext = m_pImmediateContext.Get();
		pFbx->m_pMeshImp = I_ObjectMgr.Load(listname[iObj]);
		pFbx->m_DrawList.resize(pFbx->m_pMeshImp->m_DrawList.size());
		pFbx->SetPosition(T::TVector3(iObj * 100.0f, 0, 0));
		for (int iDraw = 0; iDraw < pFbx->m_pMeshImp->m_DrawList.size(); iDraw++)
		{
			pFbx->m_pMeshImp->m_DrawList[iDraw]->m_pContext = m_pImmediateContext.Get();
		}
	}

	m_FbxObj[0].m_pAnimImporter = m_FbxObj[1].m_pMeshImp;
	return true;
}
bool	Sample::Init()
{
	HRESULT hr;
	if (FAILED(m_QuadObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"Quad.hlsl")))
	{
		MessageBox(0, _T("m_QuadObj 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	LoadMap();
	//LoadFbx();


	m_QuadObj.CreateTextures(m_pd3dDevice.Get(), 
			m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height);
	m_QuadObj.SetBuffer(m_pd3dDevice.Get());
	m_QuadObj.ComputeKernel(9);

	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 100.0f, -300.0f), T::TVector3(0, 0.0f, 0));
	m_pMainCamera->CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 5000.0f);
	m_pLightTex = I_Texture.Load(L"../../data/pung00.dds");

	return true;
}
bool	Sample::Frame()
{
	m_QuadObj.Frame();
	m_MapObj.Frame();
	m_Quadtree.Update(m_pMainCamera);
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Frame();
	}
	return true;
}
bool	Sample::Render()
{
	RenderIntoBuffer(m_pImmediateContext.Get());
	//m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDSSDepthDisable, 0x00);
	ApplySS(m_pImmediateContext.Get(), TDxState::m_pSSLinear);
	ApplySS(m_pImmediateContext.Get(), TDxState::g_pSSClampLinear);
	m_QuadObj.SetMatrix(nullptr, nullptr, nullptr);
	m_QuadObj.Render();

	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));

	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	return true;
}
bool	Sample::Release()
{
	m_QuadObj.Release();
	m_MapObj.Release();

	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Release();
	}
	return true;
}
void Sample::RenderIntoBuffer(ID3D11DeviceContext* pContext)
{
	// get the old render targets
	ID3D11RenderTargetView* pOldRTV;
	ID3D11DepthStencilView* pOldDSV;
	pContext->OMGetRenderTargets(1, &pOldRTV, &pOldDSV);

	// 이전 백버퍼 랜더링 결과를 사용(스카이박스)하기 위해서 알파사용.
	float color[4] ={	0, 0, 0, 0	};
	pContext->ClearRenderTargetView(m_QuadObj.m_pColorRTV.Get(), color);
	pContext->ClearRenderTargetView(m_QuadObj.m_pNormalDepthRTV.Get(), color);
	pContext->ClearDepthStencilView(m_QuadObj.m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	// Set the new render targets
	ID3D11RenderTargetView* pViews[2];
	pViews[0] = m_QuadObj.m_pColorRTV.Get();
	pViews[1] = m_QuadObj.m_pNormalDepthRTV.Get();
	pContext->OMSetRenderTargets(2, pViews, m_QuadObj.m_pDepthStencilView.Get());

	// Render the particles
	RenderMRT(pContext);

	// restore the original render targets
	pViews[0] = pOldRTV;
	pViews[1] = NULL;
	pContext->OMSetRenderTargets(2, pViews, pOldDSV);
	SAFE_RELEASE(pOldRTV);
	SAFE_RELEASE(pOldDSV);
}
void Sample::RenderMRT(ID3D11DeviceContext* pContext)
{
	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDSSDepthEnable, 0x00);
	ApplyBS(m_pImmediateContext.Get(), TDxState::m_AlphaBlendDisable);
	
	m_MapObj.m_bAlphaBlend = false;
	m_MapObj.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Quadtree.Render();

	if (m_pLightTex)
	{
		m_pImmediateContext->PSSetShaderResources(
			1, 1, m_pLightTex->m_pSRV.GetAddressOf());
	}
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_FbxObj[iObj].Render();
	}
	ClearD3D11DeviceContext(pContext);
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();