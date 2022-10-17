#include "TSceneTitle.h"
#include "TInput.h"
bool     TSceneTitle::IsNextScene()
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
	
	m_pBoxObj = new TShapeBox;
	m_pBoxObj->Create(m_pd3dDevice, m_pImmediateContext, L"DefaultObject.txt", L"../../data/gameHeight.png");

	m_pMainCamera = new TCamera;
	m_pMainCamera->CreateViewMatrix(TVector(0,0,-10), TVector(0, 0, 0), TVector(0,1, 0) );
	m_pMainCamera->CreateProjMatrix(1.0f, 100.0f, T_PI * 0.5f,
									(float)g_rtClient.right/ (float)g_rtClient.bottom);
	return true;
}
bool TSceneTitle::Frame()
{	
	m_pMainCamera->Frame();
	m_pBoxObj->Frame();
	return true;
}
bool TSceneTitle::Render()
{
	//m_pBG->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	//m_pBG->Render();	
	m_pBoxObj->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pBoxObj->Render();
	return true;
}
bool TSceneTitle::Release()
{
	m_pBG->Release();	
	m_pBoxObj->Release();
	delete m_pBoxObj;
	return true;
}