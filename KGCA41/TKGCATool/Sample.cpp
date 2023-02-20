#include "pch.h"
#include "Sample.h"

void  Sample::NewEffect(UINT iParticleCounter, T_STR tex)
{
	TParticleObj* p = new TParticleObj;	
	std::wstring shaderfilename = L"Particle.txt";
	if (iParticleCounter <= 0) iParticleCounter = 1;
	p->m_iParticleCounter = iParticleCounter;
	p->m_Paticles.resize(iParticleCounter);
	p->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),
		shaderfilename,
		tex);
	
	/*p->m_matScale = TMatrix::CreateScale(10.0f);

	p->m_matTranslate._41 = randstep(-10.0f, +10.0f);
	p->m_matTranslate._42 = randstep(-10.0f, +10.0f);
	p->m_matTranslate._43 = randstep(-10.0f, +10.0f);*/
	m_ParticleList.push_back(p);
}
bool  Sample::GetIntersection()
{
	//if (m_bObjectPicking)
	{
		if (I_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
		{
			for (auto node : m_Quadtree.m_pDrawLeafNodeList)
			{
				UINT index = 0;
				UINT iNumFace = node->m_IndexList.size() / 3;
				for (UINT face = 0; face < iNumFace; face++)
				{
					UINT i0 = node->m_IndexList[index + 0];
					UINT i1 = node->m_IndexList[index + 1];
					UINT i2 = node->m_IndexList[index + 2];
					TVector3 v0 = m_Quadtree.m_pMap->m_VertexList[i0].p;
					TVector3 v1 = m_Quadtree.m_pMap->m_VertexList[i1].p;
					TVector3 v2 = m_Quadtree.m_pMap->m_VertexList[i2].p;
					if (m_Select.ChkPick(v0, v1, v2))
					{										
						return true;
					}
					index += 3;
				}
			}
		}		
	}
	return false;
}

bool Sample::CreateMapData(UINT iColumn, UINT iRows)
{
	if (m_pTitle)
	{
		m_Quadtree.Release();
		m_pTitle->DeletaMap();

		m_pTitle->CreateMap(iColumn, iRows);
		m_Quadtree.Create(
			((TSceneTitle*)m_pCurrentScene.get())->m_pMainCamera,
			((TSceneTitle*)m_pCurrentScene.get())->m_pMap);
	}	
	return true;
}
bool Sample::LoadFbx(T_STR filepath, TVector3 vPos)
{
	TCharacter* pCharacter = I_Object.Load(filepath, L"");	
	pCharacter->m_matWorld._41 = vPos.x;
	pCharacter->m_matWorld._42 = vPos.y;
	pCharacter->m_matWorld._43 = vPos.z;
	TVector3 vMin = TVector3(-10, -10, -10) + vPos;
	TVector3 vMax = TVector3( 10,  10,  10) + vPos;
	pCharacter->m_tBox.Set(vMax, vMin);
	/*TActionTable action;
	action.iStartFrame = 61;
	action.iEndFrame = 91;
	action.bLoop = true;
	pCharacter->m_ActionList.insert(std::make_pair(L"walk", action));
	pCharacter->m_ActionCurrent = pCharacter->m_ActionList.find(L"walk")->second;*/

	m_NpcList.push_back(pCharacter);
	m_Quadtree.AddObject(pCharacter);
	return true;
}
bool Sample::CreateFbxLoader()
{
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
	m_UserCharacter->m_pd3dDevice = m_pd3dDevice.Get();
	m_UserCharacter->m_pImmediateContext = m_pImmediateContext.Get();	
	/*m_UserCharacter->m_pFbxFile = m_fbxList[m_UserCharacter->m_iFbxListID];*/
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
	m_UserCharacter->CreateConstantBuffer();

	for (int iObj = 0; iObj < 5; iObj++)
	{
		TCharacter* pNpc = new TCharacter;
		pNpc->m_pd3dDevice = m_pd3dDevice.Get();
		pNpc->m_pImmediateContext = m_pImmediateContext.Get();		
		//pNpc->m_pFbxFile = m_fbxList[pNpc->m_iFbxListID];
		pNpc->m_matWorld._41 = -4.0f + iObj * 2;
		pNpc->m_AnimScene = pNpc->m_pFbxFile->m_AnimScene;
		pNpc->CreateConstantBuffer();
		TActionTable action;
		action.iStartFrame = 61;
		action.iEndFrame = 91;
		action.bLoop = true;
		pNpc->m_ActionList.insert(std::make_pair(L"walk", action));
		pNpc->m_ActionCurrent = pNpc->m_ActionList.find(L"walk")->second;

		m_NpcList.push_back(pNpc);
	}

	return true;
}
bool Sample::Init()
{	
	I_Object.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	if (m_pTitle==nullptr)
	{
		m_pTitle = std::make_shared<TSceneTitle>();
		m_pTitle->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
		m_pTitle->Init();
	}
	if (m_pInGame == nullptr)
	{
		m_pInGame = std::make_shared<TSceneInGame>();
		m_pInGame->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
		m_pInGame->Init();
	}
	m_pCurrentScene = m_pTitle;

	//CreateFbxLoader();

	std::wstring shaderfilename = L"DefaultShape.hlsl";
	m_DirLine.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), shaderfilename,
		L"../../data/gameHeight.png");
	D3DXMatrixScaling(&m_DirLine.m_matWorld, 1000.0f, 1000.0f, 1000.0f);
	return true;
}
bool Sample::Frame()
{		
	if (m_pCurrentScene == nullptr) return true;
	m_Select.SetMatrix(nullptr, 
		&m_pCurrentScene->m_pMainCamera->m_matView,
		&m_pCurrentScene->m_pMainCamera->m_matProj);

	if (m_bObjectPicking)
	{
		if (GetIntersection())
		{
			if (m_pTitle && m_pTitle->m_pMap)
			{
				LoadFbx(m_szSelectFbxFile, m_Select.m_vIntersection);
			};
		}
	}
	if (m_bUpPicking)
	{
		if (GetIntersection())
		{
			if (m_pTitle && m_pTitle->m_pMap)
			{
				std::vector<TNode*> nodelist;
				T_BOX box;
				TVector3 vMin, vMax;
				TVector3 vRange(30, 30, 30);
				vMax = m_Select.m_vIntersection + vRange;
				vMin = m_Select.m_vIntersection - vRange;
				box.Set(vMax, vMin);
				if (m_Quadtree.SelectVertexList(box, nodelist) > 0)
				{
					for (auto node : nodelist)
					{
						for (UINT iVertex = 0; iVertex < m_Quadtree.m_pMap->m_VertexList.size(); iVertex++)
						{
							TVector3 v0 = m_Quadtree.m_pMap->m_VertexList[iVertex].p;							
							TVector3 v = v0 - m_Select.m_vIntersection;
							float fDistance = D3DXVec3Length(&v);
							if (fDistance <= 30.0f)
							{
								float fValue = (fDistance / 30.0f) * 90.0f;
								float fdot = cosf(DegreeToRadian(fValue));
								m_Quadtree.m_pMap->m_VertexList[iVertex].p.y += fdot;
								m_Quadtree.m_pMap->ComputeVertexNormal(iVertex);
							}							
						}
					}
					m_Quadtree.m_pMap->UpdateVertexBuffer();
				}
			}
		}
		
	}

	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	if (m_pCurrentScene->IsNextScene())
	{
		m_pCurrentScene = m_pInGame;
	}
	m_pCurrentScene->Frame();

	for (auto data : m_ParticleList)
	{
		data->Frame();
	}

	/*for (auto npc : m_NpcList)
	{
		npc->UpdateFrame(m_pImmediateContext.Get());
	}*/
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
	if (m_UserCharacter)
	{
		m_UserCharacter->UpdateFrame(m_pImmediateContext.Get());
	}
	return true;
}
bool Sample::Render()
{		
	if (m_pCurrentScene == nullptr) return true;
	if (m_bWireFrame)
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	else
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);

	TSceneTitle* pScene = (TSceneTitle*)m_pCurrentScene.get();

	// particle
	m_pImmediateContext->OMSetBlendState(TDxState::g_pDualSourceBlend, 0, -1);
	m_pImmediateContext->OMSetDepthStencilState(
		TDxState::g_pDefaultDepthStencilAndNoWrite,
		0xff);
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	TMatrix matBillboard;
	//TMatrix::CreateBillboard(); 
	matBillboard = m_pCurrentScene->m_pMainCamera->m_matView.Invert();
	matBillboard._41 = 0.0f;
	matBillboard._42 = 0.0f;
	matBillboard._43 = 0.0f;
	for (auto data : m_ParticleList)
	{
		//TMatrix matWorld = TMatrix::CreateRotationZ(g_fGameTimer);
		//matWorld = matBillboard * matWorld;
		// matworld = s* r* t;
		data->SetMatrix(&matBillboard,
			&pScene->m_pMainCamera->m_matView, 
			&pScene->m_pMainCamera->m_matProj);
		data->Render();
	}

	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_pImmediateContext->OMSetDepthStencilState(
		TDxState::g_pDefaultDepthStencil,0xff);
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pImmediateContext->GSSetShader(nullptr, NULL, 0);

	TVector3 vLightPos(0, 10, -50);
	TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);
	//vLight = vLight * matRotation;
	D3DXVec3TransformCoord(&vLightPos, &vLightPos, &matRotation);

	TVector3 vLightDir;
	D3DXVec3Normalize(&vLightDir, &-vLightPos);

	TMatrix matWorld;
	if (pScene->m_pUser)
	{
		matWorld._41 = pScene->m_pUser->m_vPos.x;
		matWorld._42 = pScene->m_pUser->m_vPos.y;
		matWorld._43 = pScene->m_pUser->m_vPos.z;
	}

	/*for (int iNpc = 0; iNpc < m_NpcList.size(); iNpc++)
	{	
		m_NpcList[iNpc]->SetMatrix(nullptr, &pScene->m_pMainCamera->m_matView, &pScene->m_pMainCamera->m_matProj);
		m_NpcList[iNpc]->Render();
	}*/

	if (m_UserCharacter)
	{
		m_UserCharacter->SetMatrix(&matWorld, &pScene->m_pMainCamera->m_matView, &pScene->m_pMainCamera->m_matProj);
		m_UserCharacter->Render();		
	}

	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil, 0xff);
	
	if (pScene->m_pMap)
	{
		pScene->m_pMap->m_cbData.vLightDir = 
			TVector4(vLightDir.x, vLightDir.y, vLightDir.z, 55.0f) ;
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
			&pScene->m_pMainCamera->m_matView,
			&pScene->m_pMainCamera->m_matProj);
		m_Quadtree.Frame();
		m_Quadtree.Render();
	}
	m_pCurrentScene->Render();
	m_DirLine.SetMatrix(nullptr, &m_pCurrentScene->m_pMainCamera->m_matView,
		&m_pCurrentScene->m_pMainCamera->m_matProj);
	m_DirLine.Render();		
	return true;
}
bool Sample::Release()
{	
	for (auto data : m_ParticleList)
	{
		data->Release();
		delete data;
	}
	/*for (auto npc : m_NpcList)
	{
		npc->Release();
		delete npc;
	}
	for (auto fbx : m_fbxList)
	{
		fbx->Release();
		delete fbx;
	}*/
	if (m_UserCharacter)
	{
		m_UserCharacter->Release();
		delete m_UserCharacter;
	}

	m_DirLine.Release();
	if(m_pTitle!=nullptr)m_pTitle->Release();
	if(m_pInGame != nullptr)m_pInGame->Release();

	
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