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
	
	std::vector<std::string> listname;
	listname.push_back("../../data/fbx/MultiCameras.fbx");
	listname.push_back("../../data/fbx/SM_Barrel.fbx");	
	listname.push_back("../../data/fbx/st00sc00.fbx");
	listname.push_back("../../data/fbx/SM_Tree_Var01.fbx");
	listname.push_back("../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");

	m_FbxObj.resize(listname.size());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbxLoader* pFbx = &m_FbxObj[iObj];		
		pFbx->SetPosition(T::TVector3(iObj * 100.0f,0,0));
		pFbx->m_fSpeed = (iObj+1)* 2.0f * 0.5f;
		pFbx->Init();
		pFbx->Load(listname[iObj]);
		pFbx->CreateConstantBuffer(m_pd3dDevice.Get());

		TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
		TShader* pVShader = I_Shader.CreateVertexShader(
			m_pd3dDevice.Get(), L"Box.hlsl", "VS");
		TShader* pPShader = I_Shader.CreatePixelShader(
			m_pd3dDevice.Get(), L"Box.hlsl", "PS");
		for (int iObj = 0; iObj < pFbx->m_DrawList.size(); iObj++)
		{
			pFbx->m_DrawList[iObj]->Init();
			pFbx->m_DrawList[iObj]->m_pColorTex = 
				I_Texture.Load(pFbx->m_DrawList[iObj]->m_szTexFileName);
			pFbx->m_DrawList[iObj]->m_pVShader = pVShader;
			pFbx->m_DrawList[iObj]->m_pPShader = pPShader;
			if (!pFbx->m_DrawList[iObj]->Create(
				m_pd3dDevice.Get(),
				m_pImmediateContext.Get()))
			{
				return false;
			}
		}
	}

	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 25.0f, -50.0f),
		T::TVector3(0, 0.0f, 0));

	return true;
}
bool	Sample::Frame()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbxLoader* pFbx = &m_FbxObj[iObj];		
		pFbx->m_fTime += g_fSecPerFrame * 30 * pFbx->m_fDir * pFbx->m_fSpeed;
		if (pFbx->m_fTime >= 50.0f)
		{
			pFbx->m_fDir *= -1.0f;
		}
		if (pFbx->m_fTime <= 0.0f)
		{
			pFbx->m_fDir *= -1.0f;
		}
		int iFrame = pFbx->m_fTime;
		iFrame = max(0, min(50, iFrame));
		for (int iObj = 0; iObj < pFbx->m_TreeList.size(); iObj++)
		{
			TFbxObj* pObject = pFbx->m_TreeList[iObj];
			if (pObject->m_AnimTrack.size() > 0)
			{
				pFbx->m_TreeList[iObj]->m_matAnim = pObject->m_AnimTrack[iFrame].matTrack;
			}
			if (pObject->m_AnimTrack.size() > 0)
			{
				pFbx->m_matBoneArray.matBoneWorld[iObj] =
					pObject->m_AnimTrack[iFrame].matTrack;
			}
			T::D3DXMatrixTranspose(
				&pFbx->m_matBoneArray.matBoneWorld[iObj],
				&pFbx->m_matBoneArray.matBoneWorld[iObj]);
		}

		m_pImmediateContext.Get()->UpdateSubresource(
			pFbx->m_pBoneCB, 0, NULL,
			&pFbx->m_matBoneArray, 0, 0);

	}
	return true;
}
bool	Sample::Render()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		TFbxLoader* pFbx = &m_FbxObj[iObj];
		m_pImmediateContext.Get()->VSSetConstantBuffers(
			2, 1, &pFbx->m_pBoneCB);
		
		for (int iObj = 0; iObj < pFbx->m_DrawList.size(); iObj++)
		{
			TFbxObj* pFbxObj = pFbx->m_DrawList[iObj];
			T::TVector3 vLight(cosf(g_fGameTimer) * 100.0f,
				100,
				sinf(g_fGameTimer) * 100.0f);

			T::D3DXVec3Normalize(&vLight, &vLight);
			vLight = vLight * -1.0f;
			pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
			pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
			pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
			pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;
		
			//T::D3DXMatrixIdentity(&pFbxObj->m_matWorld);
			
			pFbxObj->SetMatrix(
				&pFbx->m_matWorld,				
				&m_pMainCamera->m_matView,
				&m_pMainCamera->m_matProj);
			pFbxObj->Render();
		}
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
	m_FbxObj[0].Release();
	m_FbxObj[1].Release();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();