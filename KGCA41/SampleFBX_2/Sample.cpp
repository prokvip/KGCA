#include "Sample.h"
#include "ICoreStd.h"
// fbx sdk
// 1)fbxsdk download -> install
// - C:\Program Files\Autodesk\FBX\FBX SDK\2020.2.1
// 2)ȯ�漳��
//#include <fbxsdk.h>
//#pragma comment(lib, "libfbxsdk-md.lib")
//#pragma comment(lib, "libxml2-md.lib")
//#pragma comment(lib, "zlib-md.lib")

bool Sample::Init()
{
	// fbx 

	NEW_FBX fbximport1 = std::make_shared<TFbxImport>();
	fbximport1->Init();
	fbximport1->Load(L"../../res/fbx/box.fbx");

	NEW_FBX fbximport2 = std::make_shared<TFbxImport>();
	fbximport2->Init();
	fbximport2->Load(L"../../res/fbx/MultiCameras.FBX");

	NEW_FBX fbximport3 = std::make_shared<TFbxImport>();
	fbximport3->Init();
	fbximport3->Load(L"../../res/fbx/sphere.FBX");

	m_pFbxList.push_back(fbximport1);
	m_pFbxList.push_back(fbximport2);
	m_pFbxList.push_back(fbximport3);

	m_BoxObj = std::make_shared<TObject>();
	m_BoxObj->Set(m_pDevice, m_pImmediateContext);
	m_BoxObj->m_VertexList.resize(m_pFbxList[1]->m_tMeshList[0].m_TriangleList.size());
	for (int v = 0; v < m_pFbxList[1]->m_tMeshList[0].m_TriangleList.size(); v++)
	{
		m_BoxObj->m_VertexList[v] = m_pFbxList[1]->m_tMeshList[0].m_TriangleList[v];
	}
	m_BoxObj->Create(L"../../res/topdownmap.jpg",L"../../res/shader/DefaultObj.hlsl");



	m_pTex = I_Tex.Load(L"../../res/000.jpg");

	TMapInfo info = {
		129, 129, 1,
		L"../../res/topdownmap.jpg",
		L"../../res/shader/DefaultObj.hlsl"
	};
	m_pMapObj = new TMap;
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->Load(info);

	m_Quadtree = std::make_shared<TQuadtree>();
	m_Quadtree->Build(m_pMapObj);

	m_pDebugCamera = std::make_shared<TDebugCamera>();
	m_pDebugCamera->Init();

	m_pDebugCamera->CreatePerspectiveFov(T_PI * 0.25, (float)
		g_dwClientWidth / (float)g_dwClientHeight,
		1.0f, 300.0f);

	m_pDebugCamera->CreateLookAt({ 0,100,-1 }, { 0,0,0 });
	

	ICore::g_pMainCamera = m_pDebugCamera.get();

	return true;
}
bool Sample::Frame()
{
	m_pMapObj->Frame();
	return true;
}
bool Sample::Render()
{	
	TMatrix matWorld;
	m_pMapObj->SetMatrix(&matWorld,
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matProj);
	//m_pMapObj->Render();

	//m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilStateDisable.Get(), 1);
	TMatrix matWorld2;
	matWorld2._42 = -10.0f;	
	m_pMapObj->SetMatrix(&matWorld2,
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matProj);
	/*m_pMapObj->PreRender();
	m_pTex->Apply(m_pImmediateContext, 0);
	m_pMapObj->PostRender();*/

	m_BoxObj->SetMatrix(nullptr,
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matProj);
	m_BoxObj->Render();
	return true;
}
bool Sample::Release()
{
	auto deleteObj = [](NEW_FBX p) { p->Release(); };
	std::for_each(begin(m_pFbxList), end(m_pFbxList), deleteObj);


	m_BoxObj->Release();
	m_pDebugCamera->Release();
	if(m_pMapObj) m_pMapObj->Release();
	delete m_pMapObj;
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