#include "Sample.h"
bool Sample::Init()
{	
	I_Tex.SetDevice(m_pd3dDevice,m_pImmediateContext );
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";

	for (int iObj = 0; iObj < 10; iObj++)
	{
		TBaseObject* pObj = new TBaseObject;
		pObj->Create(m_pd3dDevice, 
					m_pImmediateContext,	
					shaderfilename,
					L"../../data/kgcabk.bmp");
		m_ObjectList.push_back(pObj);

		m_ObjectTextureList.push_back(I_Tex.Load(L"../../data/kgcabk.bmp"));
	}
	for (int iObj = 0; iObj < 10; iObj++)
	{
		TBaseObject* pObj = new TBaseObject;
		pObj->Create(m_pd3dDevice,
			m_pImmediateContext,
			shaderfilename,
			L"../../data/kgcabk.bmp");
		m_ObjectList.push_back(pObj);
		m_ObjectTextureList.push_back(I_Tex.Load(L"../../data/bitmap1.bmp"));
	}
	

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;// 최근점 필터링
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HRESULT hr = m_pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSS);
	return true;
}
bool Sample::Frame()
{		
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj]->Frame();
	}
	return true;
}
bool Sample::Render()
{	
	m_pImmediateContext->PSSetSamplers(0, 1, &g_pDefaultSS);
	//m_Texture.Apply(m_pImmediateContext,0);
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectTextureList[iObj]->Apply(m_pImmediateContext, 0);
		m_ObjectList[iObj]->Render();
	}
	return true;
}
bool Sample::Release()
{	
	m_Texture.Release();
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj]->Release();
		delete m_ObjectList[iObj];
	}
	for (int iObj = 0; iObj < m_ObjectTextureList.size(); iObj++)
	{
		m_ObjectTextureList[iObj]->Release();
		delete m_ObjectTextureList[iObj];
	}
	return true;
}

GAME_RUN(TSampleTexture, 800,600)