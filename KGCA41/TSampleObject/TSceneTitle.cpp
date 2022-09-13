#include "TSceneTitle.h"
bool TSceneTitle::Init()
{
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	m_pMap = new TMapObject;
	m_pMap->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/kgcabk.bmp");
	return true;
}
bool TSceneTitle::Frame()
{	
	return true;
}
bool TSceneTitle::Render()
{
	m_pMap->Render();
	return true;
}
bool TSceneTitle::Release()
{
	m_pMap->Release();
	return true;
}