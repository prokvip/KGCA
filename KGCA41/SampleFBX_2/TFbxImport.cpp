#include "TFbxImport.h"

FbxVector2 TFbxImport::ReadTextureCoord(FbxLayerElementUV* uvLayer, int uvIndex)
{
	FbxVector2 uv;
	switch (uvLayer->GetMappingMode())
	{
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (uvLayer->GetReferenceMode())
		{
			case FbxLayerElementUV::eDirect: //배열에서 직접 얻는다.
			{

			}break;
			case FbxLayerElementUV::eIndexToDirect:// 인덱스를 통해서 배열에 접근해서 얻는다.
			{

			}break;
		}
	}break;
	// 정점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv = uvLayer->GetDirectArray().GetAt(uvIndex);
		}break;
		}
	}break;
	}
	return uv;
}
void      TFbxImport::PreProcess(FbxNode* fbxNode)
{
	if (fbxNode == nullptr) return;
	C_STR name =fbxNode->GetName();
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	if (fbxMesh != nullptr)
	{
		m_pNodeMeshList.push_back(fbxNode);
	}
	UINT iNumChild = fbxNode->GetChildCount();
	for (int iChild = 0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChildNode = fbxNode->GetChild(iChild);		
		PreProcess(pChildNode);
	}

	
}
bool      TFbxImport::Load(T_STR filename)
{
	C_STR name = wtm(filename);
	bool ret = m_pFbxImporter->Initialize(name.c_str());
	ret = m_pFbxImporter->Import(m_pFbxScene);

	FbxNode* m_FbxRootNode = m_pFbxScene->GetRootNode();
	if (m_FbxRootNode)
	{
		// tree 순회(Tree traverse)
		PreProcess(m_FbxRootNode);
	}
	for (int iNode = 0; iNode < m_pNodeMeshList.size(); iNode++)
	{
		TFbxMesh tMesh;
		LoadMesh(m_pNodeMeshList[iNode], tMesh);
		m_tMeshList.push_back(tMesh);
	}

	return true;
}
// vb, ib, worldmatrix, texture
void	  TFbxImport::LoadMesh(FbxNode* fbxNode, TFbxMesh& tMesh)
{
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	UINT    iNumPolyCount = fbxMesh->GetPolygonCount();
	FbxVector4* pVertexPositions = fbxMesh->GetControlPoints();

	UINT iNumLayerCount = fbxMesh->GetLayerCount();
	std::vector<FbxLayerElementUV*> VertexUVSets;
	for (int iLayer = 0; iLayer < iNumLayerCount; iLayer++)
	{
		FbxLayer* layer = fbxMesh->GetLayer(iLayer);
		if (layer->GetUVs() != nullptr)
		{
			VertexUVSets.push_back(layer->GetUVs());
		}
	}


	// 1) polygon -> face 2개 인 경우 -> 폴리곤사이즈 4개 정점으로 구성.
	// 2) polygon -> face 1개 인 경우 -> 폴리곤사이즈 3개 정점으로 구성.
	for (int iPoly = 0; iPoly<iNumPolyCount; iPoly++)
	{
		UINT iNumPolySize = fbxMesh->GetPolygonSize(iPoly);
		UINT iNumTriangleCount = iNumPolySize - 2;
		
		UINT iCornerIndices[3];
		for (int iTriangle = 0; iTriangle < iNumTriangleCount; iTriangle++)
		{			
			iCornerIndices[0] = fbxMesh->GetPolygonVertex(iPoly, 0);
			iCornerIndices[1] = fbxMesh->GetPolygonVertex(iPoly, iTriangle+2);
			iCornerIndices[2] = fbxMesh->GetPolygonVertex(iPoly, iTriangle+1);

			int u[3];
			u[0] = fbxMesh->GetTextureUVIndex(iPoly, 0);
			u[1] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
			u[2] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

			for (int iVertex = 0; iVertex < 3; iVertex++)
			{				
				UINT iDCIndex = iCornerIndices[iVertex];
				PNCT_Vertex pnct;
				FbxVector4 v = pVertexPositions[iDCIndex];
				pnct.p.x = v.mData[0];
				pnct.p.y = v.mData[2];
				pnct.p.z = v.mData[1];

				pnct.n = { 0,0,0 };
				pnct.c = { 1,1,1,1 };

				if (VertexUVSets.size() > 0)
				{
					FbxLayerElementUV* uvLayer = VertexUVSets[0];
					FbxVector2 uv(0, 0);
					uv = ReadTextureCoord(uvLayer, u[iVertex]);
					pnct.t = { (float)uv.mData[0],(float)(1.0f-uv.mData[1]) };
				}
				tMesh.m_TriangleList.push_back(pnct);
			}
		}
	}
}
bool      TFbxImport::Init()
{
	m_pSDKManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pSDKManager, IOSROOT);
	m_pFbxScene = FbxScene::Create(m_pSDKManager, "");
	return true;
}
bool      TFbxImport::Release()
{	
	if (m_pFbxScene)	m_pFbxScene->Destroy();
	if (m_pFbxImporter) m_pFbxImporter->Destroy();
	if (m_pSDKManager)	m_pSDKManager->Destroy();

	m_pFbxScene = nullptr;
	m_pFbxImporter = nullptr;
	m_pSDKManager = nullptr;
	return true;
}