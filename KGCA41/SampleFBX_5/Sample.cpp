#include "Sample.h"
#include "ICoreStd.h"
#include "TModelMgr.h"
// fbx sdk
// 1)fbxsdk download -> install
// - C:\Program Files\Autodesk\FBX\FBX SDK\2020.2.1
// 2)환경설정
//#include <fbxsdk.h>
//#pragma comment(lib, "libfbxsdk-md.lib")
//#pragma comment(lib, "libxml2-md.lib")
//#pragma comment(lib, "zlib-md.lib")

bool Sample::Init()
{
	// fbx 
	//TModelMgr::Get().Load(L"../../res/fbx/MultiCameras.FBX");
	//TModelMgr::Get().Load(L"../../res/fbx/sphereBox.FBX");
	TFbxObj* pFbxObj= TModelMgr::Get().Load(L"../../res/fbx/Turret_Deploy1.FBX");
	//TModelMgr::Get().Load(L"../../res/fbx/box.fbx");
	//TModelMgr::Get().Load(L"../../res/fbx/ship.FBX");

	m_MapObj = std::make_shared<TMapObject>();
	m_MapObj->m_pFbxObject = pFbxObj;
	m_MapObj->Set(m_pDevice, m_pImmediateContext);

	auto tFbxMeshList = m_MapObj->m_pFbxObject->m_tMeshList;

	for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
	{
		TFbxMesh* fbxMesh = tFbxMeshList[iSub].get();
		NEW_OBJECT obj  = std::make_shared<TMapObject>();
		m_MapObj->m_pSubObject.push_back(obj);
		obj->Set(m_pDevice, m_pImmediateContext);		
		obj->m_ptMesh = fbxMesh;	
		
		W_STR filename;
		W_STR defaultPath = L"../../res/fbx/";
		if (fbxMesh->m_szTextureFileName.size() > 1)
		{
			filename = fbxMesh->m_szTextureFileName[0];
			defaultPath += filename;
			obj->Create(defaultPath, L"../../res/shader/DefaultObj.hlsl");
		}
		else
		{
			obj->Create(L"", L"../../res/shader/DefaultObj.hlsl");
		}

		obj->m_matWorld = fbxMesh->m_matWorld;

		for (int isubMtrl = 0; isubMtrl < fbxMesh->m_szTextureFileName.size(); isubMtrl++)
		{
			W_STR filename = fbxMesh->m_szTextureFileName[isubMtrl];
			W_STR defaultPath = L"../../res/fbx/";
			defaultPath += filename;
			obj->LoadTextureArray(defaultPath);
		}
	}	

	m_pDebugCamera = std::make_shared<TDebugCamera>();
	m_pDebugCamera->Init();
	m_pDebugCamera->CreatePerspectiveFov(T_PI * 0.25, (float)
		g_dwClientWidth / (float)g_dwClientHeight,
		1.0f, 30000.0f);
	m_pDebugCamera->CreateLookAt({ 0,0,-50 }, { 0,0,0 });
	ICore::g_pMainCamera = m_pDebugCamera.get();

	return true;
}
bool Sample::Frame()
{
	/*m_pMapObj->Frame();

	m_MapObj;->m_fCurrentAnimTime += m_MapObj;->m_iFrameSpeed *g_fSecondPerFrame;
	if (m_MapObj;->m_fCurrentAnimTime >= m_MapObj;->m_iEndFrame)
	{
		m_MapObj;->m_fCurrentAnimTime = m_MapObj;->m_iStartFrame;
	}
	for (int iSub = 0; iSub < m_MapObj;->m_pChild.size(); iSub++)
	{
		NEW_OBJECT obj = m_MapObj;->m_pChild[iSub];
		obj->m_matWorld = obj->m_ptMesh->m_MatrixArray[(int)m_MapObj;->m_fCurrentAnimTime];
	}*/
	
	return true;
}
bool Sample::Render()
{	
	//TMatrix matWorld;
	//m_pMapObj->SetMatrix(&matWorld,
	//	&ICore::g_pMainCamera->m_matView,
	//	&ICore::g_pMainCamera->m_matProj);
	////m_pMapObj->Render();

	////m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilStateDisable.Get(), 1);
	//TMatrix matWorld2;
	//matWorld2._42 = -10.0f;	
	//m_pMapObj->SetMatrix(&matWorld2,
	//	&ICore::g_pMainCamera->m_matView,
	//	&ICore::g_pMainCamera->m_matProj);
	///*m_pMapObj->PreRender();
	//m_pTex->Apply(m_pImmediateContext, 0);
	//m_pMapObj->PostRender();*/

	//for (int iSub = 0; iSub < m_MapObj;->m_pChild.size(); iSub++)
	//{
	//	NEW_OBJECT obj = m_MapObj;->m_pChild[iSub];
	//	obj->SetMatrix(nullptr,
	//		&ICore::g_pMainCamera->m_matView,
	//		&ICore::g_pMainCamera->m_matProj);
	//	obj->Render();
	//}
	
	return true;
}
bool Sample::Release()
{
	/*auto deleteObj = [](NEW_FBX p) { p->Release(); };
	std::for_each(begin(m_pFbxImporter), end(m_pFbxImporter), deleteObj);

	for (int iSub = 0; iSub < m_MapObj;->m_pChild.size(); iSub++)
	{
		NEW_OBJECT obj = m_MapObj;->m_pChild[iSub];
		obj->Release();
	}
	m_MapObj;->Release();

	m_pDebugCamera->Release();
	if(m_pMapObj) m_pMapObj->Release();
	delete m_pMapObj;*/
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