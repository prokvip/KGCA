#include "Sample.h"

bool Sample::Init()
{	m_pTitle = std::make_shared<TSceneTitle>();
	m_pInGame = std::make_shared<TSceneInGame>(); 
	m_pTitle->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
	m_pInGame->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
	m_pTitle->Init();
	m_pInGame->Init();
	m_pCurrentScene = m_pTitle;

	std::wstring shaderfilename = L"DefaultShape.txt";	
	m_DirLine.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), shaderfilename,
										L"../../data/gameHeight.png");
	
	m_DirLine.m_matWorld.Scale(1000.0f, 1000.0f, 1000.0f);

	m_RT.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), 2048,2048);
	return true;
}
bool Sample::Frame()
{		
	if (m_pCurrentScene->IsNextScene())
	{
		m_pCurrentScene = m_pInGame;
	}
	m_pCurrentScene->Frame();
	return true;
}
bool Sample::Render()
{	
	// 랜더타켓 지정
	m_RT.m_pOldRTV = m_pRTV.Get();
	m_RT.m_pOldDSV = m_pDepthStencilView.Get();
	m_RT.m_vpOld[0] = m_vp;

	if( m_RT.Begin(m_pImmediateContext.Get()))
	{
		if (I_Input.GetKey('V') == KEY_HOLD)
		{
			m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
		}
		m_pCurrentScene->Render();
		m_DirLine.SetMatrix(nullptr, &m_pCurrentScene->m_pMainCamera->m_matView,
			&m_pCurrentScene->m_pMainCamera->m_matProj);
		m_DirLine.Render();
		// 랜더타켓 복원
		m_RT.End(m_pImmediateContext.Get());
	}

	if (m_RT.m_pSRV)
	{
		m_BG.m_pTextureSRV = m_RT.m_pSRV.Get();
	}
	return true;
}
bool Sample::Release()
{	
	m_DirLine.Release();
	m_pTitle->Release();
	m_pInGame->Release();
	return true;
}

GAME_RUN(TSampleCollisionTest, 800,600)