#include "TFbxObj.h"
void	TFbxObj::ParseNode(FbxNode* pNode)
{
	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	if (pFbxMesh != nullptr)
	{
		FbxLayerElementUV* pUVList = nullptr;
		// 정점성분 리스트
		FbxLayer* pLayer = pFbxMesh->GetLayer(0);
		if (pLayer->GetVertexColors() != nullptr) {}
		if (pLayer->GetNormals() != nullptr) {}
		if (pLayer->GetTangents() != nullptr) {}
		if (pLayer->GetUVs() != nullptr) {
			pUVList = pLayer->GetUVs();
		}

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			// 삼각형, 사각형
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iCornerIndex[3];
			for (int iTriangle = 0; 
				iTriangle < m_iNumTriangle;
				iTriangle++)
			{
				// 위치 인덱스
				iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);
				// UV 인덱스
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

				for (int iIndex = 0;
					iIndex < 3;
					iIndex++)
				{
					PNCT_VERTEX vertex;
					FbxVector4 pos = pVertexPositions[iCornerIndex[iIndex]];
					vertex.pos.x = pos.mData[0];
					vertex.pos.y = pos.mData[2];
					vertex.pos.z = pos.mData[1];

					if (pUVList != nullptr)
					{
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, pUVList, 
							iCornerIndex[iIndex], u[iIndex]	);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f-uv.mData[1];
					}
					m_pVertexList.push_back(vertex);
				}
			}
		}
	}
}
FbxVector2 TFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 uv(0, 0);
	if (dwVertexTextureCount < 1 || pUVSet == nullptr)
	{
		return uv;
	}
	int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
	FbxLayerElementUV* pFbxLayerElementUV = pFbxMesh->GetElementUV(0);

	// 제어점은 평면의 4개 정점, 폴리곤 정점은 6개 정점을 위미한다.
	// 그래서 텍스처 좌표와 같은 레이어 들은 제어점 또는 정점으로 구분된다.
	switch (pUVSet->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // 제어점 당 1개의 텍스처 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: // 배열에서 직접 얻는다.
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect: // 배열에 해당하는 인덱스를 통하여 얻는다.
		{
			int id = pUVSet->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex: // 정점 당 1개의 매핑 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
	return uv;
}
void	TFbxObj::PreProcess(FbxNode* pNode)
{
	// pNode 정보 얻기
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);	
		FbxNodeAttribute::EType type = 
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh )
		{
			ParseNode(pChildNode);
		}
		PreProcess(pChildNode);
	}
}
bool	TFbxObj::LoadObject(std::string filename)
{
	// todo  : 메모리 관리;
	m_pFbxManager = FbxManager::Create();
	// 익스포터 or 임포터;
	FbxIOSettings* ios = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(ios);
	//todo  : FbxImporter* m_pFbxImporter;
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager,"");
	//todo  : FbxScene* m_pFbxScene;
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	INT iFileFormat = -1;
	bool bRet = m_pFbxImporter->Initialize( filename.c_str(), 
								iFileFormat,
								m_pFbxManager->GetIOSettings());
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	FbxNode* m_pRootNode=m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode);
		
	Create(L"FbxShader.hlsl", L"../../data/shader/DefaultShader.hlsl" );
	return bRet;
}
bool TFbxObj::Create(std::wstring vsFile, std::wstring psFile)
{
	if (CreateVertexData() )
	{
		CreateConstantBuffer();
		CreateVertexBuffer();
		//CreateIndexBuffer();
		LoadShader(vsFile, psFile);
		CreateVertexLayout();
		return true;
	}
	return false;
}
bool TFbxObj::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
	pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->Draw(m_pVertexList.size(), 0);
	return true;
}