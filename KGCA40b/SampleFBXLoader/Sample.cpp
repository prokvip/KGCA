#include "Sample.h"

//Remote: error: File lib / libfbxsdk - md.lib is 257.27 MB; this exceeds GitHub's file size limit of 100.00 MB        
//Remote: error: File lib / libfbxsdk - mt.lib is 282.44 MB; this exceeds GitHub's file size limit of 100.00 MB  


void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
bool	Sample::Init()
{	
	m_FbxObj.Init();
	//m_FbxObj.Load("../../data/fbx/SM_Barrel.fbx");
	m_FbxObj.Load("../../data/fbx/SM_Rock.fbx");

	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PS");
	for (int iObj = 0; iObj < m_FbxObj.m_ObjList.size(); iObj++)
	{		
		m_FbxObj.m_ObjList[iObj]->Init();
		m_FbxObj.m_ObjList[iObj]->m_pColorTex = I_Texture.Load(m_FbxObj.m_ObjList[iObj]->m_szTexFileName);
		m_FbxObj.m_ObjList[iObj]->m_pVShader = pVShader;
		m_FbxObj.m_ObjList[iObj]->m_pPShader = pPShader;
		m_FbxObj.m_ObjList[iObj]->SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
		if (!m_FbxObj.m_ObjList[iObj]->Create(
			m_pd3dDevice.Get(),
			m_pImmediateContext.Get()))
		{
			return false;
		}
	}


	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 25.0f, -50.0f),
		T::TVector3(0, 0.0f, 0));

	return true;
}
bool	Sample::Frame()
{	
	return true;
}
bool	Sample::Render()
{	
	for (int iObj = 0; iObj < m_FbxObj.m_ObjList.size(); iObj++)
	{
		m_FbxObj.m_ObjList[iObj]->SetMatrix(nullptr, 
			&m_pMainCamera->m_matView, 
			&m_pMainCamera->m_matProj);
		m_FbxObj.m_ObjList[iObj]->Render();
	}
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));
	return true;
}
bool	Sample::Release()
{
	m_FbxObj.Release();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();