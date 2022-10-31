#include "TFbxLoader.h"
bool TFbxLoader::Init()
{
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");	
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	return true;
}
bool TFbxLoader::Load( C_STR filename)
{	
	m_pFbxImporter->Initialize(filename.c_str());
	m_pFbxImporter->Import(m_pFbxScene);
	m_pRootNode = m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode);

	for (auto mesh : m_pFbxMeshList)
	{
		ParseMesh(mesh);
	}
	return true;
}
void TFbxLoader::ParseMesh(FbxMesh* pFbxMesh)
{
	FbxNode* pNode = pFbxMesh->GetNode();
	TFbxObject* pObject = new TFbxObject;
	
	// Layer 개념
	FbxLayerElementUV* VertexUVSet=nullptr;
	FbxLayer* pFbxLayer  = pFbxMesh->GetLayer(0);
	if (pFbxLayer->GetUVs() != nullptr)
	{
		VertexUVSet=pFbxLayer->GetUVs();
	}

	std::string szFileName;
	//int iNumMtrl = pNode->GetMaterialCount();
	//for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		// 24 이상의 정보가 있다.
		FbxSurfaceMaterial* pSurface = pNode->GetMaterial(0);
		if (pSurface)
		{
			auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (property.IsValid())
			{
				const FbxFileTexture* tex = property.GetSrcObject<FbxFileTexture>(0);
				szFileName = tex->GetFileName();
			}
		}
	}
	pObject->m_szTextureName = I_Tex.GetSplitName(szFileName);
	
	int iNumPolyCount = pFbxMesh->GetPolygonCount();
	// 3 정점 -> 1폴리곤( 삼각형)
	// 4 정점 -> 1폴리곤( 쿼드 )
	int iNumFace = 0;

	// 제어점
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iNumFace = iPolySize - 2;
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// 정점인덱스
			int iCornerIndex[3];
			iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
			iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iFace+2);
			iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iFace+1);

			int iUVIndex[3];
			iUVIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
			iUVIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 2);
			iUVIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 1);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				int vertexID = iCornerIndex[iIndex];
				FbxVector4 v = pVertexPositions[vertexID];
				SimpleVertex tVertex;
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];
				tVertex.c = TVector4(1,1,1,1);
				FbxVector2 t = ReadTextureCoord(
							pFbxMesh, 
							VertexUVSet, 
							iCornerIndex[iIndex],
							iUVIndex[iIndex]);
				tVertex.t.x = t.mData[0];
				tVertex.t.y = 1.0f-t.mData[1];

				pObject->m_VertexList.push_back(tVertex);
			}
		}
	}

	m_pDrawObjList.push_back(pObject);
}
FbxVector2 TFbxLoader::ReadTextureCoord(FbxMesh* pFbxMesh, 
	FbxLayerElementUV* pUVSet,
	int vertexIndex,
	int uvIndex)
{
	FbxVector2 t;
	/*enum EMappingMode
	{
		eNone,				// 매핑이 결정되지 않았다.
		eByControlPoint,	// 제어점 및 정점에 1개의 매핑이 되어 있다.
		eByPolygonVertex,	// 각 정점이 속한 폴리곤의 매핑에 좌표가 있다.
		eByPolygon,         // 전체 폴리곤에 매핑 좌표가 있다.
		eByEdge,			// 에지에 1개의 매핑 좌표가 있다.
		eAllSame			// 전체 폴리곤에 1개의 매핑 좌표가 있다.
	};*/
	//enum EReferenceMode
	//{
	//	eDirect,   // n번째 매핑정보가 mDirectArray의 n번째 위치에 있다.
	//	eIndex,    // fbx 5.0 이하 버전에서 사용되었었다. 이상 버전에서는 eIndexToDirect로 대체되었다.
	//	eIndexToDirect
	//};
	// 텍스처 매핑 방식이 뭐냐?
	FbxLayerElement::EMappingMode mode = pUVSet->GetMappingMode();
	switch(mode)
	{ 
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pUVSet->GetReferenceMode())
			{
			case FbxLayerElementUV::eDirect:
			{
				t = pUVSet->GetDirectArray().GetAt(vertexIndex);
			}break;
			case FbxLayerElementUV::eIndexToDirect:
			{
				int index = pUVSet->GetIndexArray().GetAt(vertexIndex);
				t = pUVSet->GetDirectArray().GetAt(index);
			}break;
			}break;
		} break;
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pUVSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				case FbxLayerElementUV::eIndexToDirect:
				{
					t = pUVSet->GetDirectArray().GetAt(uvIndex);
				}break;
			}break;
		}break;
	}
	return t;
}
void TFbxLoader::PreProcess(FbxNode* pFbxNode)
{
	if (pFbxNode == nullptr) return;
	FbxMesh* pFbxMesh = pFbxNode->GetMesh();
	if (pFbxMesh)
	{
		m_pFbxMeshList.push_back(pFbxMesh);
	}
	int iNumChild = pFbxNode->GetChildCount();
	for( int iChild=0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChild = pFbxNode->GetChild(iChild);
		PreProcess(pChild);
	}
}
bool TFbxLoader::Frame()
{
	for (auto obj : m_pDrawObjList)
	{
		obj->Frame();
	}
	return true;
}
bool TFbxLoader::Render()
{
	for (auto obj : m_pDrawObjList)
	{
		obj->Render();
	}
	return true;
}

bool TFbxLoader::Release()
{
	for (auto obj : m_pDrawObjList)
	{
		obj->Release();
	}
	
	m_pFbxScene->Destroy();

	if (m_pFbxImporter!=nullptr)
	{
		m_pFbxImporter->Destroy();
		m_pFbxImporter = nullptr;
	}
	if (m_pFbxManager != nullptr)
	{
		m_pFbxManager->Destroy();
		m_pFbxManager = nullptr;
	}
	return true;
}