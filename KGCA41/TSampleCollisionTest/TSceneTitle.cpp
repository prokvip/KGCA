#include "TSceneTitle.h"
bool TSceneTitle::Init()
{
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	m_pMapTitle = new TBaseObject;
	m_pMapTitle->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/kgcabk.bmp");
	return true;
}
bool TSceneTitle::Frame()
{	
	m_pMapTitle->Rotation();
	return true;
}
bool TSceneTitle::Render()
{
	m_pMapTitle->Render();
	return true;
}
bool TSceneTitle::Release()
{
	m_pMapTitle->Release();
	return true;
}