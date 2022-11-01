#include "Sample.h"

bool	Sample::Init()
{
	TFbxLoader* pFbxLoaderC = new TFbxLoader;
	if (pFbxLoaderC->Init())
	{
		pFbxLoaderC->Load("../../data/fbx/MultiCameras.fbx");
	}
	m_fbxList.push_back(pFbxLoaderC);

	TFbxLoader* pFbxLoaderA = new TFbxLoader;
	if (pFbxLoaderA->Init())
	{
		pFbxLoaderA->Load("../../data/fbx/box.fbx");
	}
	m_fbxList.push_back(pFbxLoaderA);

	TFbxLoader* pFbxLoaderB = new TFbxLoader;
	if (pFbxLoaderB->Init())
	{
		pFbxLoaderB->Load("../../data/fbx/sm_rock.fbx");
	}
	m_fbxList.push_back(pFbxLoaderB);

	W_STR szDefaultDir = L"../../data/fbx/";
	std::wstring shaderfilename = L"../../data/shader/DefaultObject.txt";

	for (auto fbx : m_fbxList)
	{
		for (int iObj = 0; iObj < fbx->m_pDrawObjList.size(); iObj++)
		{
			TFbxObject* pObj = fbx->m_pDrawObjList[iObj];
			std::wstring  szLoad = szDefaultDir + pObj->m_szTextureName;
			pObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),	shaderfilename, szLoad);			
		}
	}

	m_pMainCamera = new TCameraDebug;
	m_pMainCamera->CreateViewMatrix(TVector(50, 6, -50), TVector(0, 6, 0), TVector(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 1000.0f, T_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool	Sample::Frame() 
{
	m_pMainCamera->Frame();
	for (auto fbx : m_fbxList)
	{
		fbx->Frame();
	}
	return true;
}
bool	Sample::Render() 
{
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	 
	TVector vLight(0, 0, 1);
	TMatrix matRotation;
	matRotation.RotationY(g_fGameTimer);
	vLight = vLight * matRotation;
	vLight.Normalized();

	for (int iModel=0; iModel < m_fbxList.size(); iModel++)
	{
		for (int iObj = 0; iObj < m_fbxList[iModel]->m_pDrawObjList.size(); iObj++)
		{
			TFbxObject* pObj = m_fbxList[iModel]->m_pDrawObjList[iObj];
			TMatrix matWorld;
			matWorld._41 = 100 * iModel;

			pObj->m_cbData.x = vLight.x;
			pObj->m_cbData.y = vLight.y;
			pObj->m_cbData.z = vLight.z;
			pObj->SetMatrix(&matWorld,&m_pMainCamera->m_matView,&m_pMainCamera->m_matProj);
			pObj->Render();			
		}
	}
	m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);
	return true;
}
bool	Sample::Release() 
{
	for (auto fbx : m_fbxList)
	{
		fbx->Release();
	}
	return true;
}
GAME_RUN(TFBXLoader, 800, 600)