#include "Sample.h"

bool Sample::Init()
{
	m_pTitle = new TSceneTitle;
	m_pInGame = new TSceneInGame;
	m_pTitle->Create(m_pd3dDevice, m_pImmediateContext, L"");
	m_pInGame->Create(m_pd3dDevice, m_pImmediateContext, L"");
	m_pTitle->Init();
	m_pInGame->Init();
	m_pCurrentScene = m_pInGame;
	return true;
}
bool Sample::Frame()
{		
	if (I_Input.GetKey(VK_F1) == KEY_PUSH)
	{
		m_pCurrentScene = m_pInGame;
	}
	m_pCurrentScene->Frame();
	return true;
}
bool Sample::Render()
{	
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	m_pCurrentScene->Render();	
	return true;
}
bool Sample::Release()
{	
	m_pTitle->Release();
	m_pInGame->Release();
	delete m_pTitle;
	delete m_pInGame;	
	return true;
}

GAME_RUN(TSampleCollisionTest, 800,600)