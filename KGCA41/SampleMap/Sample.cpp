#include "Sample.h"
#include "ICoreStd.h"
float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	m_pTex = I_Tex.Load(L"../../res/000.jpg");

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
	m_pDebugCamera->CreatePerspectiveFov(T_PI * 0.25, (float)
		g_dwClientWidth / (float)g_dwClientHeight,
		1.0f, 300.0f);

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
	TMatrix matWorld;
	m_pMapObj->SetMatrix(&matWorld,
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matProj);
	m_pMapObj->Render();

	//m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilStateDisable.Get(), 1);
	TMatrix matWorld2;
	matWorld2._42 = -10.0f;	
	m_pMapObj->SetMatrix(&matWorld2,
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matProj);
	m_pMapObj->PreRender();
	m_pTex->Apply(m_pImmediateContext, 0);
	m_pMapObj->PostRender();
	return true;
}
bool Sample::Release()
{
	m_pDebugCamera->Release();
	if(m_pMapObj) m_pMapObj->Release();
	delete m_pMapObj;
	return true;
}
bool  Sample::DeleteDxResource()
{
	TCore::DeleteDxResource();
	return true;
}
bool  Sample::CreateDxResource()
{
	ICore::g_pMainCamera->CreatePerspectiveFov(T_PI * 0.25, (float)
		g_dwClientWidth / (float)g_dwClientHeight,
		1.0f, 300.0f);

	TCore::CreateDxResource();
	return true;
}
TGAME(L"kgca", 800, 600)