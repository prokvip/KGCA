#include "pch.h"
#include "TScene.h"
bool		TScene::Create(
	ID3D11Device* pd3dDevice,// ����̽� ��ü
	ID3D11DeviceContext* pImmediateContext,
	std::wstring shadername)
{
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;
	return true;
}
bool TScene::Init()
{	
	return true;
}
bool TScene::Frame()
{
	return true;
}
bool TScene::Render()
{	
	return true;
}
bool TScene::Release()
{	
	return true;
}