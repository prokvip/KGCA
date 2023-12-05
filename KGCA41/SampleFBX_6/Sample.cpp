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
void  TMapObject::SetFbxObj(TFbxObj* pObject)
{
	m_pFbxObject = pObject;
}
TFbxObj* TMapObject::GetFbxObj()
{
	return  m_pFbxObject;
}
bool Sample::Init()
{
	// fbx 
	/*TFbxObj* pFbxObj1 = TModelMgr::Get().Load(L"../../res/fbx/MultiCameras.FBX");
	TFbxObj* pFbxObj2 = TModelMgr::Get().Load(L"../../res/fbx/sphereBox.FBX");
	TFbxObj* pFbxObj3 = TModelMgr::Get().Load(L"../../res/fbx/Turret_Deploy1.FBX");
	TFbxObj* pFbxObj4 = TModelMgr::Get().Load(L"../../res/fbx/box.fbx");
	TFbxObj* pFbxObj5 = TModelMgr::Get().Load(L"../../res/fbx/ship.FBX");*/
	TFbxObj* pFbxObj1 = TModelMgr::Get().Load(L"../../res/fbx/man.FBX");

	m_MapObj = std::make_shared<TMapObject>();
	m_MapObj->SetFbxObj(TModelMgr::Get().GetPtr(L"man.FBX"));
	m_MapObj->Set(m_pDevice, m_pImmediateContext);	
	auto tFbxMeshList = m_MapObj->GetFbxObj()->m_tMeshList;

	for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
	{
		TFbxMesh* fbxMesh = tFbxMeshList[iSub].get();
		NEW_OBJECT obj  = std::make_shared<TMapObject>();
		m_MapObj->m_pChildObjectList.push_back(obj);
		obj->Set(m_pDevice, m_pImmediateContext);		
		obj->m_ptMesh = fbxMesh;	
		
		obj->Set(m_pDevice, m_pImmediateContext);
		obj->m_VertexList.resize(fbxMesh->m_iNumPolygon * 3);
		obj->m_pSubIWVertexList.resize(fbxMesh->m_iNumPolygon * 3);
		UINT iNumSubMaterial =fbxMesh->m_TriangleList.size();

		UINT iSubVertexIndex = 0;
		for (int iMtrl = 0; iMtrl < iNumSubMaterial; iMtrl++)
		{
			fbxMesh->m_TriangleOffsetList.push_back(iSubVertexIndex);
			for (int v = 0; v < fbxMesh->m_TriangleList[iMtrl].size(); v++)
			{
				obj->m_VertexList[iSubVertexIndex + v] = fbxMesh->m_TriangleList[iMtrl][v];
				obj->m_pSubIWVertexList[iSubVertexIndex + v] = fbxMesh->m_pSubIWVertexList[iMtrl][v];
			}
			iSubVertexIndex += fbxMesh->m_TriangleList[iMtrl].size();
		}

		W_STR filename;
		W_STR defaultPath = L"../../res/fbx/";
		if (fbxMesh->m_szTextureFileName.size() > 1)
		{
			filename = fbxMesh->m_szTextureFileName[0];
			defaultPath += filename;
			obj->Create(defaultPath, L"DefaultObj.hlsl");
		}
		else
		{
			obj->Create(L"", L"DefaultObj.hlsl");
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
	m_pDebugCamera->CreateLookAt({ 0,20,-50 }, { 0,20,0 });
	ICore::g_pMainCamera = m_pDebugCamera.get();

	return true;
}
bool Sample::Frame()
{	
	TFbxObj* pFbxObj = m_MapObj->GetFbxObj();
	m_MapObj->m_fCurrentAnimTime += pFbxObj->GetFrameSpeed() *g_fSecondPerFrame;
	if (m_MapObj->m_fCurrentAnimTime >= pFbxObj->GetEndFrame())
	{
		m_MapObj->m_fCurrentAnimTime = pFbxObj->GetStartFrame();
	}
	for (int iSub = 0; iSub < m_MapObj->m_pChildObjectList.size(); iSub++)
	{
		NEW_OBJECT obj = m_MapObj->m_pChildObjectList[iSub];
		obj->m_matWorld = obj->m_ptMesh->m_MatrixArray[(int)m_MapObj->m_fCurrentAnimTime];
	}	
	return true;
}
bool Sample::Render()
{	
	for (int iSub = 0; iSub < m_MapObj->m_pChildObjectList.size(); iSub++)
	{
		NEW_OBJECT obj = m_MapObj->m_pChildObjectList[iSub];
		obj->SetMatrix(nullptr,
			&ICore::g_pMainCamera->m_matView,
			&ICore::g_pMainCamera->m_matProj);
		obj->Render();
	}	
	return true;
}
bool Sample::Release()
{
	for (int iSub = 0; iSub < m_MapObj->m_pChildObjectList.size(); iSub++)
	{
		NEW_OBJECT obj = m_MapObj->m_pChildObjectList[iSub];
		obj->Release();
	}
	m_MapObj->Release();
	m_pDebugCamera->Release();
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