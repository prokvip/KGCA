#include "TFbxImport.h"

FbxVector2 TFbxImport::ReadTextureCoord(FbxLayerElementUV* uvLayer, int uvIndex)
{
	FbxVector2 uv;
	switch (uvLayer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (uvLayer->GetReferenceMode())
		{
			case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
			{

			}break;
			case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
			{

			}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
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
		// tree ��ȸ(Tree traverse)
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


	// 1) polygon -> face 2�� �� ��� -> ����������� 4�� �������� ����.
	// 2) polygon -> face 1�� �� ��� -> ����������� 3�� �������� ����.
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