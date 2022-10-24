#include "TSceneTitle.h"
#include "TInput.h"
#include "TDxState.h"
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
	
	m_pMap = new TMap;
	// 2*n½Â+1
	m_pMap->Build(512+1, 512 + 1);
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"DefaultObject.txt", L"../../data/map/Tile50.jpg");
	

	m_pBoxObjA = new TObjectBox;
	m_pBoxObjA->Create(m_pd3dDevice, m_pImmediateContext, L"DefaultObject.txt", L"../../data/object/20200428_185613.jpg");
	m_pBoxObjA->m_matWorld.Translation(0, 0, 0);

	m_pBoxObjB = new TObjectBox;
	m_pBoxObjB->Create(m_pd3dDevice, m_pImmediateContext, L"DefaultObject.txt", L"../../data/_RAINBOW.bmp");
	m_pBoxObjB->m_matWorld.Translation(1, 0, 4);

	m_pMainCamera = new TCameraDebug;
	m_pMainCamera->CreateViewMatrix(TVector(0,0,-10), TVector(0, 0, 0), TVector(0,1, 0) );
	m_pMainCamera->CreateProjMatrix(1.0f, 10000.0f, T_PI * 0.25f,
									(float)g_rtClient.right/ (float)g_rtClient.bottom);
	return true;
}
bool TSceneTitle::Frame()
{	
	//m_fYaw, m_fPitch, m_fRoll;
	if (I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
	{
		m_pMainCamera->m_fYaw += I_Input.m_ptOffset.x * 0.002f;
		m_pMainCamera->m_fPitch += I_Input.m_ptOffset.y * 0.002f;
	}
	m_pMainCamera->Frame();
	m_pBoxObjA->Frame();
	m_pBoxObjA->m_matWorld.RotationY(g_fGameTimer);
	m_pBoxObjA->m_matWorld._41 = m_pBoxObjA->m_vPos.x;
	m_pBoxObjA->m_matWorld._42 = m_pBoxObjA->m_vPos.y;
	m_pBoxObjA->m_matWorld._43 = m_pBoxObjA->m_vPos.z;
	m_pBoxObjB->Frame();


	
	return true;
}
bool TSceneTitle::Render()
{	
	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil,
		0xff);
	m_pMap->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pMap->Render();

	TCameraDebug* pDCam = (TCameraDebug*)m_pMainCamera;
	bool bRender = pDCam->m_vFrustum.ClassifyPoint(m_pBoxObjA->m_vPos);
	if (bRender)
	{
		m_pBoxObjA->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pBoxObjA->Render();
	}
	///*m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pGreaterDepthStencil,
	//	0xff);*/
	bRender = pDCam->m_vFrustum.ClassifyPoint(m_pBoxObjB->m_vPos);
	if (bRender)
	{
		m_pBoxObjB->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pBoxObjB->Render();
	}
	return true;
}
bool TSceneTitle::Release()
{
	m_pBG->Release();	
	m_pBoxObjA->Release();
	delete m_pBoxObjA;

	m_pBoxObjB->Release();
	delete m_pBoxObjB;

	m_pMap->Release();
	delete m_pMap;
	return true;
}