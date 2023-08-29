#include "TScene.h"
#include "ICoreStd.h"

bool TSceneTitle::Init()
{
	m_pMapObj = std::make_unique<TObject>();
	m_pMapObj->Set(ICore::g_pDevice, ICore::g_pContext);
	m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pMapObj->SetScale(TVector3(g_fMapHalfSizeX, g_fMapHalfSizeY, 1.0f));
	m_pMapObj->Create(L"../../res/topdownmap.jpg", L"../../res/shader/Plane.hlsl");

	return true;
}
bool TSceneTitle::Frame()
{
	m_pMapObj->Frame();
	return true;
}
bool TSceneTitle::Render()
{
	m_pMapObj->SetMatrix(nullptr, 
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matOrthoProjection);
	m_pMapObj->Render();
	return true;
}
bool TSceneTitle::Release()
{
	m_pMapObj->Release();
	return true;
}