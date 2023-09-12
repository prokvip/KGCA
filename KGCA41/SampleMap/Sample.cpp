#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	TMapInfo info = {
		9, 9, 1,
		L"../../res/topdownmap.jpg",
		L"../../res/shader/DefaultObj.hlsl"
	};
	m_pMapObj = new TMap;
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->Load(info);

	m_pMainCamera->CreateLookAt({0,5,-1}, {0,0,0});
	m_pMainCamera->CreatePerspectiveFov(T_PI*0.25, (float)g_dwWindowWidth/ (float)g_dwWindowHeight,
		1.0f, 10000.0f);
	return true;
}
bool Sample::Frame()
{
	//m_pMapObj->Frame();
	return true;
}
bool Sample::Render()
{	
	m_pMapObj->SetMatrix(nullptr, &m_pMainCamera->m_matView,&m_pMainCamera->m_matProj);
	m_pMapObj->Render();
	return true;
}
bool Sample::Release()
{
	m_pMapObj->Release();
	delete m_pMapObj;
	return true;
}

TGAME(L"kgca", 800, 600)