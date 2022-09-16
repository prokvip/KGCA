#include "Sample.h"

bool Sample::Init()
{
	m_pTitle = new TSceneTitle;
	m_pInGame = new TSceneInGame;
	m_pTitle->Create(m_pd3dDevice, m_pImmediateContext, L"");
	m_pInGame->Create(m_pd3dDevice, m_pImmediateContext, L"");
	m_pTitle->Init();
	m_pInGame->Init();
	m_pCurrentScene = m_pTitle;

	D3D11_VIEWPORT vp;
	vp.Width = 400;
	vp.Height = 300;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);
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
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);
	m_pCurrentScene->Render();

	vp.Width = 100;
	vp.Height = 100;
	vp.TopLeftX = 0;
	vp.TopLeftY = 500;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);
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

GAME_RUN(TSampleTexture, 800, 600)