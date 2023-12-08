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
	std::wstring szShaderFile = L"DefaultObj.hlsl";
	TModelMgr::Get().Set(m_pDevice, m_pImmediateContext);
	// fbx 
	TFbxObj* pFbxObj[6];	
	pFbxObj[0] = TModelMgr::Get().Load(L"../../res/fbx/man.FBX", L"CharacterWeight.hlsl");
	pFbxObj[1] = TModelMgr::Get().Load(L"../../res/fbx/Turret_Deploy1.FBX", szShaderFile);
	pFbxObj[2] = TModelMgr::Get().Load(L"../../res/fbx/box.fbx", szShaderFile);
	pFbxObj[3] = TModelMgr::Get().Load(L"../../res/fbx/sphereBox.FBX", szShaderFile);
	pFbxObj[4] = TModelMgr::Get().Load(L"../../res/fbx/ship.FBX", szShaderFile);
	pFbxObj[5] = TModelMgr::Get().Load(L"../../res/fbx/MultiCameras.FBX", szShaderFile);

	auto obj1 = std::make_shared<TMapObjSkinning>();
	obj1->m_pModel = TModelMgr::Get().GetPtr(L"man.FBX");
	obj1->CreateBoneBuffer();
	m_MapObj.push_back(obj1);

	for (int i = 1; i < 6; i++)
	{
		auto obj2 = std::make_shared<TMapObj>();
		obj2->m_pModel = pFbxObj[i];
		obj2->CreateBoneBuffer();
		m_MapObj.push_back(obj2);
	}

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
	if (I_Input.GetKey(VK_HOME) == KEY_PUSH)
	{
		auto obj = std::make_shared<TMapObj>();
		obj->m_pModel = TModelMgr::Get().GetPtr(L"Turret_Deploy1.FBX");
		obj->CreateBoneBuffer();
		m_MapObj.push_back(obj);
	}
	for (auto obj : m_MapObj)
	{
		obj->Frame();
	}
	return true;
}
bool Sample::Render()
{	
	int index = 0;
	for (auto obj : m_MapObj)
	{
		obj->m_matControl.Translation(TVector3(index *100.0f, 0, 0));
		obj->Render();
		index++;
	}

	return true;
}
bool Sample::Release()
{
	for (auto obj : m_MapObj)
	{
		obj->Release();
	}

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