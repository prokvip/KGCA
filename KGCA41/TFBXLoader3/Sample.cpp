#include "Sample.h"
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
bool	Sample::Init()
{
	/*TFbxFile* pFbxLoaderC = new TFbxFile;
	if (pFbxLoaderC->Init())
	{
		pFbxLoaderC->Load("../../data/fbx/MultiCameras.fbx");
	}
	m_fbxList.push_back(pFbxLoaderC);*/

	

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
	std::wstring shaderfilename = L"Skinning.txt";

	for (auto fbx : m_fbxList)
	{
		for (int iObj = 0; iObj < fbx->m_pDrawObjList.size(); iObj++)
		{
			TFbxObject* pObj = fbx->m_pDrawObjList[iObj];
			std::wstring  szLoad = szDefaultDir + pObj->m_szTextureName;
			pObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),	shaderfilename, szLoad);			
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
	
	m_pMainCamera = new TCameraDebug;
	m_pMainCamera->CreateViewMatrix(TVector3(0, 0, -10), TVector3(0, 0, 0), TVector3(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 10000.0f, T_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool	Sample::Frame() 
{
	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	m_pMainCamera->Frame();
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
bool	Sample::Render() 
{
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	 
	TVector3 vLight(0, 0, 1);
	TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);
	//vLight = vLight * matRotation;
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&vLight, &vLight);

	for (int iNpc=0; iNpc < m_NpcList.size(); iNpc++)
	{				
		m_NpcList[iNpc]->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_NpcList[iNpc]->Render(m_pImmediateContext.Get());
	}

	m_UserCharacter->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_UserCharacter->Render(m_pImmediateContext.Get());
	m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);
	return true;
}
bool	Sample::Release() 
{
	delete m_pMainCamera;
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
	return true;
}
GAME_RUN(TFBXLoader, 800, 600)