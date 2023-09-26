#pragma once
#include <TObject.h>
#include "TFbxImport.h"


class TModelMgr
{
public:
	static TModelMgr& Get()
	{
		static TModelMgr mgr;
		return mgr;
	}
	TFbxImport	m_pFbxImporter;
	TFbxObj* Load(W_STR filename)
	{
		m_pFbxImporter.Init();
		std::shared_ptr<TFbxObj> loadobj=std::make_shared<TFbxObj>();
		if (m_pFbxImporter.Load(filename, loadobj.get()))
		{			
			for (int iSub = 0; iSub < loadobj->m_tMeshList.size(); iSub++)
			{
				TFbxMesh& fbxMesh = *(loadobj->m_tMeshList[iSub].get());
				loadobj->m_VertexList.resize(fbxMesh.m_iNumPolygon * 3);				
				UINT iNumSubMaterial =	fbxMesh.m_TriangleList.size();

				UINT iSubVertexIndex = 0;
				for (int iMtrl = 0; iMtrl < iNumSubMaterial; iMtrl++)
				{
					fbxMesh.m_TriangleOffsetList.push_back(iSubVertexIndex);
					for (int v = 0; v < fbxMesh.m_TriangleList[iMtrl].size(); v++)
					{
						loadobj->m_VertexList[iSubVertexIndex + v] = fbxMesh.m_TriangleList[iMtrl][v];
					}
					iSubVertexIndex += fbxMesh.m_TriangleList[iMtrl].size();
				}	

				//vb
			}
			m_tFbxObjList.insert(std::make_pair(filename, loadobj));
			return loadobj.get();
		}

		m_pFbxImporter.Release();
		return nullptr;
	}
public:
	std::map<W_STR, std::shared_ptr<TFbxObj>>  m_tFbxObjList;
	TFbxObj* GetPtr(W_STR key)
	{
		return nullptr;
	}
};


