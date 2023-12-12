#include "pch.h"
#include "TModelMgr.h"
// 1) Load()  랜더링 오브젝틀 작업.
//    - 인스턴스 오브젝트에서 활용가능하다.
// 2) 재 출력 및 로드
TFbxObj* TModelMgr::GetPtr(W_STR key)
{
	auto iter = m_tFbxObjList.find(key);
	if (m_tFbxObjList.end() == iter)
	{
		return nullptr;
	}
	return iter->second.get();
}

TFbxObj* TModelMgr::Load(W_STR szPullfilePath, W_STR szShaderFile)
{
	std::size_t found = szPullfilePath.find_last_of(L"/");
	std::wstring path = szPullfilePath.substr(0, found + 1);
	std::wstring key = szPullfilePath.substr(found + 1);
	TFbxObj* data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}
	TFbxObj* ret = nullptr;
	m_pFbxImporter.Init();
	std::shared_ptr<TFbxObj> loadobj = std::make_shared<TFbxObj>();
	if (m_pFbxImporter.Load(szPullfilePath, loadobj.get()))
	{
		loadobj->Set(m_pDevice, m_pContext);
		for (int iSub = 0; iSub < loadobj->m_DrawList.size(); iSub++)
		{
			TFbxObj* fbxMesh = loadobj->m_DrawList[iSub].get();
			fbxMesh->Set(m_pDevice, m_pContext);
			fbxMesh->m_VertexList.resize(fbxMesh->m_iNumPolygon * 3);
			fbxMesh->m_pVertexListIW.resize(fbxMesh->m_iNumPolygon * 3);
			UINT iNumSubMaterial = fbxMesh->m_TriangleList.size();

			UINT iSubVertexIndex = 0;
			for (int iMtrl = 0; iMtrl < iNumSubMaterial; iMtrl++)
			{
				fbxMesh->m_TriangleOffsetList.push_back(iSubVertexIndex);
				for (int v = 0; v < fbxMesh->m_TriangleList[iMtrl].size(); v++)
				{
					fbxMesh->m_VertexList[iSubVertexIndex + v] = fbxMesh->m_TriangleList[iMtrl][v];
					fbxMesh->m_pVertexListIW[iSubVertexIndex + v] = fbxMesh->m_pSubIWVertexList[iMtrl][v];
				}
				iSubVertexIndex += fbxMesh->m_TriangleList[iMtrl].size();
			}

			W_STR filename;
			W_STR defaultPath = L"../../res/fbx/";
			if (fbxMesh->m_szTextureFileName.size() > 1)
			{
				filename = fbxMesh->m_szTextureFileName[0];
				defaultPath += filename;
				fbxMesh->Create(defaultPath, szShaderFile);
			}
			else
			{
				fbxMesh->Create(L"", szShaderFile);
			}

			fbxMesh->m_matWorld = fbxMesh->m_matWorld;

			for (int isubMtrl = 0; isubMtrl < fbxMesh->m_szTextureFileName.size(); isubMtrl++)
			{
				W_STR filename = fbxMesh->m_szTextureFileName[isubMtrl];
				W_STR defaultPath = L"../../res/fbx/";
				defaultPath += filename;
				fbxMesh->LoadTextureArray(defaultPath);
			}
		}

		m_tFbxObjList.insert(std::make_pair(key, loadobj));
		ret = loadobj.get();
	}
	//m_pFbxImporter.Write();
	m_pFbxImporter.Release();
	return ret;
}
//TFbxObj* TModelMgr::LoadFormKgcFile(W_STR szPullfilePath)
//{
//	std::size_t found = szPullfilePath.find_last_of(L"/");
//	std::wstring path = szPullfilePath.substr(0, found + 1);
//	std::wstring key = szPullfilePath.substr(found + 1);
//	TFbxObj* data = GetPtr(key);
//	if (data != nullptr)
//	{
//		return data;
//	}
//	TFbxObj* ret = nullptr;
//	m_pKgcImporter.Init();
//	std::shared_ptr<TFbxObj> loadobj = std::make_shared<TFbxObj>();
//	if (m_pFbxImporter.Load(szPullfilePath, loadobj.get()))
//	{
//
//		m_tFbxObjList.insert(std::make_pair(key, loadobj));
//		ret = loadobj.get();
//	}
//	//m_pFbxImporter.Write();
//	m_pFbxImporter.Release();
//	return ret;
//}