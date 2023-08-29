#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{	
	m_pTitleScene = std::make_shared<TSceneTitle>();
	m_pScene= m_pTitleScene.get();

	m_pMainCamera->Create({ 0.0f,0.0f, 0.0f }, { (float)m_dwWindowWidth, (float)m_dwWindowHeight });
	m_pScene->Init();
	return true;
}
bool Sample::Frame()
{
	m_pScene->Frame();
	return true;
}
bool Sample::Render()
{	
	m_pScene->SetMatrix(nullptr, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matOrthoProjection);
	m_pScene->Render();
	return true;
}
bool Sample::Release()
{
	m_pScene->Release();
	return true;
}

TGAME(L"kgca", 800, 600)