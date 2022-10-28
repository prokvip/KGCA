#include "Sample.h"

bool	Sample::Init()
{
	if (m_FBXLoader.Init())
	{
		m_FBXLoader.Load("../../data/fbx/box.fbx");
	}

	std::wstring shaderfilename = L"../../data/shader/DefaultObject.txt";
	for (int iObj = 0; iObj < m_FBXLoader.m_pDrawObjList.size(); iObj++)
	{
		TBaseObject* pObj = m_FBXLoader.m_pDrawObjList[iObj];		
		pObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), 
			shaderfilename, L"../../data/_RAINBOW.bmp");		
	}

	m_pMainCamera = new TCameraDebug;
	m_pMainCamera->CreateViewMatrix(TVector(50, 6, -50), TVector(0, 6, 0), TVector(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 1000.0f, T_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool	Sample::Frame() 
{
	m_FBXLoader.Frame();
	return true;
}
bool	Sample::Render() 
{
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	for (int iObj = 0; iObj < m_FBXLoader.m_pDrawObjList.size(); iObj++)
	{
		m_FBXLoader.m_pDrawObjList[iObj]->SetMatrix(nullptr, 
			&m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);
		m_FBXLoader.m_pDrawObjList[iObj]->Render();	
	}
	m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);
	return true;
}
bool	Sample::Release() 
{
	m_FBXLoader.Release();
	return true;
}
GAME_RUN(TFBXLoader, 800, 600)