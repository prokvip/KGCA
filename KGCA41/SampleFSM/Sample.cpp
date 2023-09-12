#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{	
	m_pTitleScene = std::make_shared<TSceneTitle>();
	m_pTitleScene->m_szBackround = L"../../res/KGCABK.bmp";
	m_pTitleScene->Init();
	m_pScene= m_pTitleScene.get();

	m_pTitleLobby = std::make_shared<TSceneLobby>();
	m_pTitleLobby->m_szBackround = L"../../res/ground.png";
	m_pTitleLobby->Init();
	m_pMainCamera->Create({ 0.0f,0.0f, 0.0f }, { (float)m_dwWindowWidth, (float)m_dwWindowHeight });

	return true;
}
bool Sample::Frame()
{	
	if (m_pScene == nullptr) return true;
	auto pBtn = m_pScene->m_SelectObjectList.find(1);
	if (pBtn != m_pScene->m_SelectObjectList.end())
	{
		TBtnObj* btn = (TBtnObj*)pBtn->second;
		if (btn->m_iSelectState == TSelectState::T_SELECTED)
		{
			m_pScene = m_pTitleLobby.get();
		}
	}

	m_pScene->Frame();
	return true;
}
bool Sample::Render()
{	
	if (m_pScene == nullptr) return true;
	m_pScene->SetMatrix(nullptr, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matOrthoProjection);
	m_pScene->Render();
	return true;
}
bool Sample::Release()
{
	m_pTitleScene->Release();
	//m_pTitleLobby->Release();
	return true;
}

TGAME(L"kgca", 800, 600)