#include "Sample.h"
#include "ICoreStd.h"
float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	TMapInfo info = {
		129, 129, 1,
		L"../../res/topdownmap.jpg",
		L"../../res/shader/DefaultObj.hlsl"
	};
	m_pMapObj = new TMap;
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->Load(info);

	m_pDebugCamera = std::make_shared<TDebugCamera>();
	m_pDebugCamera->Init();
	m_pDebugCamera->CreateLookAt({ 0,100,-100 }, { 0,0,0 });
	m_pDebugCamera->CreatePerspectiveFov(T_PI * 0.25, (float)g_dwWindowWidth / (float)g_dwWindowHeight,
		1.0f, 10000.0f);

	ICore::g_pMainCamera = m_pDebugCamera.get();

	return true;
}
bool Sample::Frame()
{
	m_pMapObj->Frame();
	return true;
}
bool Sample::Render()
{	
	m_pMapObj->SetMatrix(nullptr, 
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matProj);
	m_pMapObj->Render();
	return true;
}
bool Sample::Release()
{
	m_pDebugCamera->Release();
	if(m_pMapObj) m_pMapObj->Release();
	delete m_pMapObj;
	return true;
}

TGAME(L"kgca", 800, 600)