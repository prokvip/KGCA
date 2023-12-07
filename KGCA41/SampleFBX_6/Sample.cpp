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
	TModelMgr::Get().Set(m_pDevice, m_pImmediateContext);
	// fbx 
	TFbxObj* pFbxObj1 = TModelMgr::Get().Load(L"../../res/fbx/box.fbx");
	TFbxObj* pFbxObj2 = TModelMgr::Get().Load(L"../../res/fbx/sphereBox.FBX");	
	TFbxObj* pFbxObj4 = TModelMgr::Get().Load(L"../../res/fbx/Turret_Deploy1.FBX");
	TFbxObj* pFbxObj5 = TModelMgr::Get().Load(L"../../res/fbx/ship.FBX");
	TFbxObj* pFbxObj3 = TModelMgr::Get().Load(L"../../res/fbx/MultiCameras.FBX");
	TFbxObj* pFbxObj6 = TModelMgr::Get().Load(L"../../res/fbx/man.FBX");

	m_MapObj = std::make_shared<TMapObj>();
	m_MapObj->SetFbxObj(TModelMgr::Get().GetPtr(L"man.FBX"));
	

	m_pDebugCamera = std::make_shared<TDebugCamera>();
	m_pDebugCamera->Init();
	m_pDebugCamera->CreatePerspectiveFov(T_PI * 0.25, (float)
		g_dwClientWidth / (float)g_dwClientHeight,
		1.0f, 30000.0f);
	m_pDebugCamera->CreateLookAt({ 0,0,-5 }, { 0,0,0 });
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

	auto tFbxMeshList = m_MapObj->GetFbxObj()->m_tMeshList;
	for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
	{
		TFbxObj* obj = tFbxMeshList[iSub].get();
		TMatrix matWorld = obj->m_MatrixArray[(int)m_MapObj->m_fCurrentAnimTime];
	}	

	// 스킨(메쉬)와 바인드포즈(에니메이션행렬)의 노드개수가 다른 수 있다.
	//TBoneWorld matAnimation;
	//for (int inode = 0; inode < pFbxObj->m_TreeList.size(); inode++)
	//{
	//	TFbxObj* pMeshModel = pFbxObj->m_TreeList[inode];
	//	// pFbxObj->m_matBoneArray.matBoneWorld[inode] = InvBondMatrix * AnimationMatrix[time];
	//	pFbxObj->m_matBoneArray.matBoneWorld[inode] =  TMatrix();
	//}


	return true;
}
bool Sample::Render()
{	
	TMatrix matWorld;
	for (int i=0; i < 10; i++)
	{
		matWorld.Translation(TVector3(i*100, 0, 0));
		auto tObj = TModelMgr::Get().GetPtr(L"man.FBX");
		//auto tFbxMeshList = m_MapObj->GetFbxObj()->m_tMeshList;
		auto tFbxMeshList = tObj->m_tMeshList;
		for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
		{
			TFbxObj* obj = tFbxMeshList[iSub].get();
			obj->SetMatrix(&matWorld,
				&ICore::g_pMainCamera->m_matView,
				&ICore::g_pMainCamera->m_matProj);
			obj->PreRender();
			obj->PostRender();
		}
	}
	return true;
}
bool Sample::Release()
{
	/*for (int iSub = 0; iSub < m_MapObj->m_pChildObjectList.size(); iSub++)
	{
		NEW_OBJECT obj = m_MapObj->m_pChildObjectList[iSub];
		obj->Release();
	}*/
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