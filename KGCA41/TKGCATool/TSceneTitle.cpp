#include "pch.h"
#include "TSceneTitle.h"
#include "TInput.h"
#include "TDxState.h"
bool TSceneTitle::DeletaMap()
{
	if (m_pMap)
	{
		m_pMap->Release();
		delete m_pMap;
		
	}
	return true;
}
bool TSceneTitle::CreateMap(UINT iColumn, UINT iRows )
{
	m_pMap = new TMap;
	// 2*n½Â+1
	//m_pMap->LoadHeightMap(m_pd3dDevice, m_pImmediateContext, L"../../data/map/heightMap513.bmp");
	m_pMap->Build(iColumn, iRows);
	//m_pMap->Build(m_pMap->m_iNumCols, m_pMap->m_iNumRows);
	//m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"DefaultMap.txt", L"../../data/map/003.jpg");
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"TessellationMap.hlsl", L"../../data/map/003.jpg");

	if (m_pUser)
	{
		TVector3 vCamera = m_pUser->m_vPos + TVector3(0, 10, -10);
		vCamera.y = m_pMap->GetHeight(vCamera.x, vCamera.z),
			m_pMainCamera->CreateViewMatrix(vCamera, m_pUser->m_vPos, TVector3(0, 1, 0));
		m_pMainCamera->CreateProjMatrix(1.0f, 1000.0f, T_PI * 0.25f,
			(float)g_rtClient.right / (float)g_rtClient.bottom);
		m_vBeforePos = m_pUser->m_vPos;
	}
	return true;
}
bool TSceneTitle::CreatePlayer()
{
	m_pUser = new TUser;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext, L"DefaultObject.txt", L"../../data/object/20200428_185613.jpg");
	D3DXMatrixTranslation(&m_pUser->m_matWorld,
		0,
		m_pMap->GetHeight(0, 0),
		0);
	m_pUser->m_vPos.x = m_pUser->m_matWorld._41;
	m_pUser->m_vPos.y = m_pUser->m_matWorld._42;
	m_pUser->m_vPos.z = m_pUser->m_matWorld._43;

	TVector3 vCamera = m_pUser->m_vPos + TVector3(0, 10, -10);
	vCamera.y = m_pMap->GetHeight(vCamera.x, vCamera.z),
		m_pMainCamera->CreateViewMatrix(vCamera, m_pUser->m_vPos, TVector3(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 1000.0f, T_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);
	m_vBeforePos = m_pUser->m_vPos;
	return true;
}
bool TSceneTitle::IsNextScene()
{
	//if (m_btnStart->m_CurrentState == UI_SELECT)
	//{
	//	return true;
	//}
	return false;
}
bool TSceneTitle::Init()
{
	I_Sprite.Load(L"SpriteInfo.txt");
	I_Sprite.Load(L"ui.txt");
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	m_pBG = new TBaseObject;
	m_pBG->Create(m_pd3dDevice,m_pImmediateContext,shaderfilename,L"../../data/gameHeight.png");
		
	m_pMapCamera = std::make_shared<TCameraMap>();
	TVector3 vCamera = TVector3(0, 0, 0);
	TVector3 vHeight = TVector3(0, 10, -10);
	m_pMapCamera->SetPos(vCamera, vHeight);	
	m_pMapCamera->CreateViewMatrix(m_pMapCamera->m_vPos, m_pMapCamera->m_vTarget, TVector3(0, 1, 0));
	m_pMapCamera->CreateProjMatrix(1.0f, 10000.0f, T_PI * 0.25f,
										(float)g_rtClient.right / (float)g_rtClient.bottom);
	m_pMapCamera->Frame();

	m_pDebugCamera = std::make_shared<TCameraDebug>();
	m_pDebugCamera->SetPos(vCamera, vHeight);
	m_pDebugCamera->CreateViewMatrix(m_pMapCamera->m_vPos, m_pMapCamera->m_vTarget, TVector3(0, 1, 0));
	m_pDebugCamera->CreateProjMatrix(1.0f, 10000.0f, T_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);

	m_pMainCamera = m_pDebugCamera.get();
	return true;
}
bool TSceneTitle::Frame()
{	
	TVector3 x = m_pMainCamera->m_vLook;
	x.y = 0;
	D3DXVec3Normalize(&x, &x);
	
	if (m_pUser)
	{
		m_pUser->m_vLook = x;
		m_pUser->m_vPos.y = m_pMap->GetHeight(m_pUser->m_vPos.x, m_pUser->m_vPos.z);
		m_pUser->Frame();
	
		TVector3 vHeight;
		vHeight.y = m_pMap->GetHeight(m_pMainCamera->m_vPos.x, m_pMainCamera->m_vPos.z);
		m_pMainCamera->SetPos(m_pUser->m_vPos, vHeight);
	}
	m_pMainCamera->Frame();
	
	return true;
}
bool TSceneTitle::Render(ID3D11DeviceContext* pContext)
{	
	TCameraDebug* pDCam = (TCameraDebug*)m_pMainCamera;
	if (m_pUser)
	{
		m_pUser->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pUser->Render(pContext);
	}
	return true;
}
bool TSceneTitle::Release()
{
	if (m_pBG)
	{
		m_pBG->Release();
		delete m_pBG;
	}
	if (m_pUser)
	{
		m_pUser->Release();
		delete m_pUser;
	}
	if (m_pMap)
	{
		m_pMap->Release();
		delete m_pMap;
	}	
	return true;
}