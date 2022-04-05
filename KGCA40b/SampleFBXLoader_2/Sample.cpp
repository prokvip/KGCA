#include "Sample.h"
#include "TObjectMgr.h"
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
	std::vector<std::wstring> listname;
	listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/SM_Barrel.fbx");	
	//listname.push_back(L"../../data/fbx/st00sc00.fbx");
	//listname.push_back(L"../../data/fbx/SM_Tree_Var01.fbx");
	listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");
	listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");

	m_FbxObj.resize(listname.size());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbxObj* pFbx = &m_FbxObj[iObj];	
		pFbx->m_pd3dDevice = m_pd3dDevice.Get();
		pFbx->m_pContext = m_pImmediateContext.Get();
		pFbx->m_pImporter = I_ObjectMgr.Load(listname[iObj]);
		pFbx->m_Scene = pFbx->m_pImporter->m_Scene;
		pFbx->SetPosition(T::TVector3(iObj * 100.0f,0,0));
		pFbx->Init();		
		pFbx->CreateConstantBuffer(m_pd3dDevice.Get());
		
		TShader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(), L"Box.hlsl", "VS");
		TShader* pPShader = I_Shader.CreatePixelShader(	m_pd3dDevice.Get(), L"Box.hlsl", "PS");
		for (int iObj = 0; iObj < pFbx->m_pImporter->m_DrawList.size(); iObj++)
		{
			TFbxModel* pModel = new TFbxModel;
			pModel->Init();
			pModel->m_pColorTex = pFbx->m_pImporter->m_DrawList[iObj]->m_pColorTex;
			pModel->m_pVShader = pVShader;
			pModel->m_pPShader = pPShader;
			pModel->m_pSubVertexList.assign(pFbx->m_pImporter->m_DrawList[iObj]->m_pSubVertexList.begin(),
											pFbx->m_pImporter->m_DrawList[iObj]->m_pSubVertexList.end());
			pModel->m_pTextureList.assign(	pFbx->m_pImporter->m_DrawList[iObj]->m_pTextureList.begin(),
											pFbx->m_pImporter->m_DrawList[iObj]->m_pTextureList.end());
			if (!pModel->Create(m_pd3dDevice.Get(),m_pImmediateContext.Get()))
			{
				return false;
			}
			pFbx->m_DrawList.push_back(pModel);
		}
	}

	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 25.0f, -50.0f),T::TVector3(0, 0.0f, 0));

	return true;
}
bool	Sample::Frame()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbxObj* pFbx = &m_FbxObj[iObj];
		pFbx->Frame();
	}
	return true;
}
bool	Sample::Render()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbxObj* pFbx = &m_FbxObj[iObj];	
		pFbx->SetMatrix(nullptr,&m_pMainCamera->m_matView,&m_pMainCamera->m_matProj);
		pFbx->Render();
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
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Release();
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();