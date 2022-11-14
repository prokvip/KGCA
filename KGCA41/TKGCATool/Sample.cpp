#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{	
	m_pTitle = std::make_shared<TSceneTitle>();
	m_pInGame = std::make_shared<TSceneInGame>(); 
	m_pTitle->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
	m_pInGame->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
	m_pTitle->Init();
	m_pInGame->Init();
	m_pCurrentScene = m_pTitle;

	m_Quadtree.Create(
		((TSceneTitle*)m_pCurrentScene.get())->m_pMainCamera,
		((TSceneTitle*)m_pCurrentScene.get())->m_pMap);

	std::wstring shaderfilename = L"DefaultShape.txt";	
	m_DirLine.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), shaderfilename,
										L"../../data/gameHeight.png");
	
	TFbxFile* pFbxLoaderD = new TFbxFile;
	if (pFbxLoaderD->Init())
	{
		if (pFbxLoaderD->Load("../../data/fbx/Man.FBX"))
		{
			pFbxLoaderD->CreateConstantBuffer(m_pd3dDevice.Get());
		}
	}
	m_fbxList.push_back(pFbxLoaderD);

	TFbxFile* pFbxLoaderA = new TFbxFile;
	if (pFbxLoaderA->Init())
	{
		if (pFbxLoaderA->Load("../../data/fbx/Swat@turning_right_45_degrees.fbx"))
		{
			pFbxLoaderA->CreateConstantBuffer(m_pd3dDevice.Get());
		}
	}
	m_fbxList.push_back(pFbxLoaderA);

	TFbxFile* pFbxLoaderB = new TFbxFile;
	if (pFbxLoaderB->Init())
	{
		pFbxLoaderB->Load("../../data/fbx/Swat.fbx");
	}
	m_fbxList.push_back(pFbxLoaderB);

	W_STR szDefaultDir = L"../../data/fbx/";
	std::wstring fbxShaderfilename = L"Skinning.txt";

	for (auto fbx : m_fbxList)
	{
		for (int iObj = 0; iObj < fbx->m_pDrawObjList.size(); iObj++)
		{
			TFbxObject* pObj = fbx->m_pDrawObjList[iObj];
			std::wstring  szLoad = szDefaultDir + pObj->m_szTextureName;
			pObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), fbxShaderfilename, szLoad);
		}
	}

	m_UserCharacter = new TCharacter;
	m_UserCharacter->m_iFbxListID = 2;
	m_UserCharacter->m_pFbxFile = m_fbxList[m_UserCharacter->m_iFbxListID];
	m_UserCharacter->m_pAnionFbxFile = pFbxLoaderA;
	if (m_UserCharacter->m_pAnionFbxFile)
	{
		m_UserCharacter->m_AnimScene = m_UserCharacter->m_pAnionFbxFile->m_AnimScene;
		m_UserCharacter->m_ActionFileList.insert(std::make_pair(L"walking", pFbxLoaderA));
		m_UserCharacter->m_ActionCurrent.iStartFrame = pFbxLoaderA->m_AnimScene.iStartFrame;
		m_UserCharacter->m_ActionCurrent.iEndFrame = 50;// pFbxLoaderA->m_AnimScene.iEndFrame;
	}
	else
	{
		m_UserCharacter->m_AnimScene = m_UserCharacter->m_pFbxFile->m_AnimScene;
		TActionTable action;
		action.iStartFrame = m_UserCharacter->m_AnimScene.iStartFrame;
		action.iEndFrame = m_UserCharacter->m_AnimScene.iEndFrame;
		action.bLoop = true;
		m_UserCharacter->m_ActionList.insert(std::make_pair(L"idle", action));
	}

	/*action.iStartFrame = 61;
	action.iEndFrame = 91;
	action.bLoop = true;
	m_UserCharacter->m_ActionList.insert(std::make_pair(L"walk", action));
	action.iStartFrame = 92;
	action.iEndFrame = 116;
	action.bLoop = true;
	m_UserCharacter->m_ActionList.insert(std::make_pair(L"run", action));
	action.iStartFrame = 120;
	action.iEndFrame = 225;
	action.bLoop = false;
	m_UserCharacter->m_ActionList.insert(std::make_pair(L"jump", action));
	action.iStartFrame = 205;
	action.iEndFrame = 289;
	action.bLoop = false;
	m_UserCharacter->m_ActionList.insert(std::make_pair(L"attack", action));*/
	m_UserCharacter->CreateConstantBuffer(m_pd3dDevice.Get());

	for (int iObj = 0; iObj < 5; iObj++)
	{
		TCharacter* pNpc = new TCharacter;
		pNpc->m_iFbxListID = 0;
		pNpc->m_pFbxFile = m_fbxList[pNpc->m_iFbxListID];
		pNpc->m_matWorld._41 = -4.0f + iObj * 2;
		pNpc->m_AnimScene = pNpc->m_pFbxFile->m_AnimScene;
		pNpc->CreateConstantBuffer(m_pd3dDevice.Get());
		TActionTable action;
		action.iStartFrame = 61;
		action.iEndFrame = 91;
		action.bLoop = true;
		pNpc->m_ActionList.insert(std::make_pair(L"walk", action));
		pNpc->m_ActionCurrent = pNpc->m_ActionList.find(L"walk")->second;

		m_NpcList.push_back(pNpc);
	}

	D3DXMatrixScaling(&m_DirLine.m_matWorld, 1000.0f, 1000.0f, 1000.0f);
	return true;
}
bool Sample::Frame()
{		
	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	if (m_pCurrentScene->IsNextScene())
	{
		m_pCurrentScene = m_pInGame;
	}
	m_pCurrentScene->Frame();

	for (auto npc : m_NpcList)
	{
		npc->UpdateFrame(m_pImmediateContext.Get());
	}
	/*if (I_Input.GetKey('J') == KEY_HOLD)
	{
		TActionTable action = m_UserCharacter->m_ActionList.find(L"jump")->second;
		m_UserCharacter->m_ActionCurrent = action;
	}
	else
	{
		TActionTable action = m_UserCharacter->m_ActionList.find(L"idle")->second;
		m_UserCharacter->m_ActionCurrent = action;
	}*/
	m_UserCharacter->UpdateFrame(m_pImmediateContext.Get());
	return true;
}
bool Sample::Render()
{		
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	TSceneTitle* pScene = (TSceneTitle*)m_pCurrentScene.get();

	TVector3 vLight(0, 0, 1);
	TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);
	//vLight = vLight * matRotation;
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&vLight, &vLight);

	TMatrix matWorld;
	matWorld._41 = pScene->m_pUser->m_vPos.x;
	matWorld._42 = pScene->m_pUser->m_vPos.y;
	matWorld._43 = pScene->m_pUser->m_vPos.z;

	for (int iNpc = 0; iNpc < m_NpcList.size(); iNpc++)
	{
		matWorld._41 += iNpc * 10.0f;
		m_NpcList[iNpc]->SetMatrix(&matWorld, &pScene->m_pMainCamera->m_matView, &pScene->m_pMainCamera->m_matProj);
		m_NpcList[iNpc]->Render(m_pImmediateContext.Get());
	}
	matWorld._41 = pScene->m_pUser->m_vPos.x;
	matWorld._42 = pScene->m_pUser->m_vPos.y;
	matWorld._43 = pScene->m_pUser->m_vPos.z;
	m_UserCharacter->SetMatrix(&matWorld, &pScene->m_pMainCamera->m_matView, &pScene->m_pMainCamera->m_matProj);
	m_UserCharacter->Render(m_pImmediateContext.Get());
	m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);

	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil, 0xff);
	
	pScene->m_pMap->SetMatrix(nullptr,
		&pScene->m_pMainCamera->m_matView,
		&pScene->m_pMainCamera->m_matProj);
	m_Quadtree.Frame();
	m_Quadtree.Render();
	
	m_pCurrentScene->Render();
	m_DirLine.SetMatrix(nullptr, &m_pCurrentScene->m_pMainCamera->m_matView,
		&m_pCurrentScene->m_pMainCamera->m_matProj);
	m_DirLine.Render();		
	return true;
}
bool Sample::Release()
{	
	for (auto npc : m_NpcList)
	{
		npc->Release();
		delete npc;
	}
	for (auto fbx : m_fbxList)
	{
		fbx->Release();
		delete fbx;
	}
	m_UserCharacter->Release();
	delete m_UserCharacter;

	m_DirLine.Release();
	m_pTitle->Release();
	m_pInGame->Release();
	return true;
}
HRESULT Sample::CreateDXResource()
{
	TGameCore::CreateDXResource();
	if (m_pCurrentScene)
	{
		m_pCurrentScene->m_pMainCamera->CreateProjMatrix(
			1.0f, 1000.0f, T_PI * 0.25f,
			(float)g_rtClient.right / (float)g_rtClient.bottom);
	}
	return S_OK;
}
void Sample::ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext)
{
	// Unbind all objects from the immediate context
	if (pd3dDeviceContext == NULL) return;

	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV = NULL;
	ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	// Shaders
	//pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->HSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->DSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	//// IA clear
	//pd3dDeviceContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
	//pd3dDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
	//pd3dDeviceContext->IASetInputLayout(NULL);

	//// Constant buffers
	//pd3dDeviceContext->VSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->HSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->DSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->GSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->PSSetConstantBuffers(0, 14, pBuffers);

	// Resources
	pd3dDeviceContext->VSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->HSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->DSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->GSSetShaderResources(0, 16, pSRVs);
	pd3dDeviceContext->PSSetShaderResources(0, 16, pSRVs);

	//// Samplers
	//pd3dDeviceContext->VSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->HSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->DSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->GSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->PSSetSamplers(0, 16, pSamplers);

	//// Render targets
	//pd3dDeviceContext->OMSetRenderTargets(8, pRTVs, pDSV);

	//// States
	//FLOAT blendFactor[4] = { 0,0,0,0 };
	//pd3dDeviceContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
	//pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	//pd3dDeviceContext->RSSetState(NULL);
}