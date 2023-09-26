#pragma once
#include <TObject.h>
#include "TFbxImport.h"

struct TFbxObj
{
	W_STR     m_csName;
	UINT	m_iStartFrame = 0;
	UINT	m_iEndFrame = 0;
	UINT	m_iFrameSpeed = 30;
	UINT	m_iTickForFrame = 160;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	std::vector< PNCT_Vertex> m_VertexList;
	std::vector< DWORD>       m_IndexList;
	std::vector<std::shared_ptr<TFbxMesh>>		    m_tMeshList;
};

class TModelMgr
{
public:
	static TModelMgr& Get()
	{
		static TModelMgr mgr;
		return mgr;
	}
	TFbxImport	m_pFbxImporter;
	bool        Load(W_STR filename)
	{
		m_pFbxImporter.Init();
		if (m_pFbxImporter.Load(filename))
		{
			std::shared_ptr<TFbxObj> loadobj;
			loadobj = std::make_shared<TFbxObj>();
			loadobj->m_iStartFrame = m_pFbxImporter.m_iStartFrame;
			loadobj->m_iEndFrame = m_pFbxImporter.m_iEndFrame;
			loadobj->m_iFrameSpeed = m_pFbxImporter.m_iFrameSpeed;
			loadobj->m_iTickForFrame = m_pFbxImporter.m_iTickForFrame;

			/*for (int iSub = 0; iSub < m_pFbxImporter.m_tMeshList.size(); iSub++)
			{
				TFbxMesh& fbxMesh = m_pFbxImporter[0]->m_tMeshList[iSub];

				std::shared_ptr<TFbxObj> obj = std::make_shared<TFbxObj>();
				m_MapObj;->m_pChild.push_back(obj);

				obj->Set(m_pDevice, m_pImmediateContext);
				obj->m_VertexList.resize(fbxMesh.m_iNumPolygon * 3);
				obj->m_ptMesh = &fbxMesh;

				UINT iNumSubMaterial =
					fbxMesh.m_TriangleList.size();

				UINT iSubVertexIndex = 0;
				for (int iMtrl = 0; iMtrl < iNumSubMaterial; iMtrl++)
				{
					fbxMesh.m_TriangleOffsetList.push_back(iSubVertexIndex);
					for (int v = 0; v < fbxMesh.m_TriangleList[iMtrl].size(); v++)
					{
						obj->m_VertexList[iSubVertexIndex + v] = fbxMesh.m_TriangleList[iMtrl][v];
					}
					iSubVertexIndex += fbxMesh.m_TriangleList[iMtrl].size();
				}

				W_STR filename;
				W_STR defaultPath = L"../../res/fbx/";
				if (fbxMesh.m_szTextureFileName.size() > 1)
				{
					filename = fbxMesh.m_szTextureFileName[0];
					defaultPath += filename;
					obj->Create(defaultPath, L"../../res/shader/DefaultObj.hlsl");
				}
				else
				{
					obj->Create(L"", L"../../res/shader/DefaultObj.hlsl");
				}


				obj->m_matWorld = fbxMesh.m_matWorld;

				for (int isubMtrl = 0; isubMtrl < fbxMesh.m_szTextureFileName.size(); isubMtrl++)
				{
					W_STR filename = fbxMesh.m_szTextureFileName[isubMtrl];
					W_STR defaultPath = L"../../res/fbx/";
					defaultPath += filename;
					obj->LoadTextureArray(defaultPath);
				}
			}*/
			m_tFbxObjList.insert(std::make_pair(filename, loadobj));
		}

		m_pFbxImporter.Release();
		return true;
	}
public:
	std::map<W_STR, std::shared_ptr<TFbxObj>>  m_tFbxObjList;

};


