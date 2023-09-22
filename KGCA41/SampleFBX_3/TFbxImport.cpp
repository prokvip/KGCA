#include "TFbxImport.h"
int TFbxImport::GetSubMaterialIndex(UINT iPoly, FbxLayerElementMaterial*  mtrl)
{
	int iSubMtrl = 0;
	switch (mtrl->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygon:
	{
		switch (mtrl->GetReferenceMode())
		{		
		case FbxLayerElementUV::eIndex: //�迭���� ���� ��´�.
		{
			iSubMtrl = iPoly;
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			iSubMtrl = mtrl->GetIndexArray().GetAt(iPoly);
		}break;
		}
	}break;
	default:
	{
		break;
	}
	}
	return iSubMtrl;
}
C_STR TFbxImport::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	C_STR texName;
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			C_STR filename = tex->GetFileName();
			return filename;
		}
	}
	return texName;
}
FbxVector4  TFbxImport::ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex)
{
	FbxVector4 normal;
	switch (layer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
		{
			normal = layer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			normal = layer->GetDirectArray().GetAt(iIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	}
	return normal;
}
FbxColor	TFbxImport::ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex, 
	int iIndex)
{
	FbxColor color(1,1,1,1);
	switch (layer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
		{
			color = layer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			color = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			color = layer->GetDirectArray().GetAt(iIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			color = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	}
	return color;
}
FbxVector2 TFbxImport::ReadTextureCoord(FbxLayerElementUV* uvLayer, int iVertexIndex, int uvIndex)
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
				uv = uvLayer->GetDirectArray().GetAt(iVertexIndex);
			}break;
			case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
			{
				int id = uvLayer->GetIndexArray().GetAt(iVertexIndex);
				uv = uvLayer->GetDirectArray().GetAt(id);
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
	std::vector<FbxLayerElementVertexColor*>	VertexColorSets;
	std::vector<FbxLayerElementNormal*>			VertexNormalSets;
	std::vector<FbxLayerElementUV*>				VertexUVSets;
	std::vector<FbxLayerElementMaterial*>		MaterialSets;
	for (int iLayer = 0; iLayer < iNumLayerCount; iLayer++)
	{
		FbxLayer* layer = fbxMesh->GetLayer(iLayer);
		if (layer->GetUVs() != nullptr)
		{
			VertexUVSets.push_back(layer->GetUVs());
		}
		if (layer->GetVertexColors() != nullptr)
		{
			VertexColorSets.push_back(layer->GetVertexColors());
		}
		if (layer->GetNormals() != nullptr)
		{
			VertexNormalSets.push_back(layer->GetNormals());
		}
		if (layer->GetMaterials() != nullptr)
		{
			MaterialSets.push_back(layer->GetMaterials());
		}
	}

	
	/// <summary>
	/// Material
	/// </summary>
	int iNumMtrl = fbxNode->GetMaterialCount();
	if (iNumMtrl > 1)
	{
		tMesh.m_TriangleList.resize(iNumMtrl);
	}
	else
	{
		tMesh.m_TriangleList.resize(1);
	}

	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pSurface =  fbxNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			std::string texturename = ParseMaterial(pSurface);
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath_s(texturename.c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
			

			std::string texName = FName;
			texName += Ext;
			tMesh.m_szTextureFileName.push_back(mtw(texName));
		}
	}




	/// <summary>
	/// GeometircMatrix
	/// </summary>
	/// <param name="fbxNode"></param>
	/// <param name="tMesh"></param>
	FbxAMatrix GeometircMatrix; // pos
	FbxAMatrix normalMatrix; // normal
	{
		FbxVector4 t = fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 s = fbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxVector4 r = fbxNode->GetGeometricRotation(FbxNode::eSourcePivot);

		GeometircMatrix.SetT(t);
		GeometircMatrix.SetR(r);
		GeometircMatrix.SetS(s);

		normalMatrix = GeometircMatrix;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();
	}

	// 1) polygon -> face 2�� �� ��� -> ����������� 4�� �������� ����.
	// 2) polygon -> face 1�� �� ��� -> ����������� 3�� �������� ����.
	//** ������͸��� ���� �� ����
	// �� ������ ���� �ش��ϴ� �ؽ�ó�� �˻��ؼ� ������ �����ؾ� �Ѵ�.
	UINT  iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly<iNumPolyCount; iPoly++)
	{
		// ������͸��� �˻�
		int iSubMtrl = 0;
		if (iNumMtrl>=1)
		{
			iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSets[0]);
		}

		UINT iNumPolySize = fbxMesh->GetPolygonSize(iPoly);
		UINT iNumTriangleCount = iNumPolySize - 2;
		
		UINT iCornerIndices[3];

		tMesh.m_iNumPolygon += iNumTriangleCount;
		for (int iTriangle = 0; iTriangle < iNumTriangleCount; iTriangle++)
		{			
			iCornerIndices[0] = fbxMesh->GetPolygonVertex(iPoly, 0);
			iCornerIndices[1] = fbxMesh->GetPolygonVertex(iPoly, iTriangle+2);
			iCornerIndices[2] = fbxMesh->GetPolygonVertex(iPoly, iTriangle+1);

			int u[3];
			u[0] = fbxMesh->GetTextureUVIndex(iPoly, 0);
			u[1] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
			u[2] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

			UINT  iColorIndex[3] = { 0, iTriangle + 2 , iTriangle + 1 };
			
			for (int iVertex = 0; iVertex < 3; iVertex++)
			{				
				UINT iDCIndex = iCornerIndices[iVertex];
				PNCT_Vertex pnct;
				FbxVector4 v = pVertexPositions[iDCIndex];
				auto finalPos = GeometircMatrix.MultT(v);
				pnct.p.x = finalPos.mData[0];
				pnct.p.y = finalPos.mData[2];
				pnct.p.z = finalPos.mData[1];

				pnct.n = { 0,0,0 };
				if (VertexNormalSets.size() > 0)
				{
					FbxVector4 normal = ReadNormal(VertexNormalSets[0],
						iDCIndex,
						iBasePolyIndex + iColorIndex[iVertex]);
					
					auto finalNormal = normalMatrix.MultT(normal);
					finalNormal.Normalize();

					pnct.n.x = finalNormal.mData[0];
					pnct.n.y = finalNormal.mData[2];
					pnct.n.z = finalNormal.mData[1];
				}

				pnct.c = { 1,1,1,1 };
				if (VertexColorSets.size() > 0)
				{
					FbxColor color = ReadColor(VertexColorSets[0], iDCIndex, 
												iBasePolyIndex + iColorIndex[iVertex]);
					pnct.c.x = color.mRed;
					pnct.c.y = color.mGreen;
					pnct.c.z = color.mBlue;
					pnct.c.w = 1.0f;
				}			

				if (VertexUVSets.size() > 0)
				{
					FbxLayerElementUV* uvLayer = VertexUVSets[0];
					FbxVector2 uv(0, 0);
					uv = ReadTextureCoord(uvLayer, iDCIndex, u[iVertex]);
					pnct.t = { (float)uv.mData[0],(float)(1.0f-uv.mData[1]) };
				}
				tMesh.m_TriangleList[iSubMtrl].push_back(pnct);
			}
		}

		iBasePolyIndex += iNumPolySize;
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