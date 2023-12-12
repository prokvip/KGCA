#include "pch.h"
#include "TFbxImport.h"

/*
GetMappingMode() 요소가 표면에 매핑되는 방식을 결정한다.
eNone			: 매핑이 결정되지 않았다.
eByControlPoint : 각 표면(surface) 제어점/정점에 대해 하나의 매핑 좌표가 있다.
eByPolygonVertex: 각 정점에 대해 하나의 매핑 좌표가 있으며, 그 일부가 되는 모든 다각형(polygon)에 대해 있다.
				  즉, 정점은 자신이 속한 다각형 만큼 많은 매핑 좌표를 갖게된다.
eByPolygon		: 전체 다각형(polygon)에 대해 하나의 매핑 좌표 만있을 수 있다.
eByEdge			: 메시의 각 고유 가장자리에 대해 하나의 매핑 좌표가 있다.
				  이것은 스무딩 레이어 요소와 함께 사용하기 위한 것이다.
eAllSame		: 전체 표면(surface)에 대해 하나의 매핑 좌표 만있을 수 있다.
*/
/*
GetReferenceMode() 매핑 정보가 좌표 배열에 저장되는 방식을 결정한다.
eDirect			: n 번째 요소의 매핑 정보가 FbxLayerElementTemplate::mDirectArray의 n 번째 위치에 있음을 나타낸다.
eIndex,			: 이 기호는 FBX v5.0 파일과의 하위 호환성을 위해 유지된다.
				  FBX v6.0 이상에서는이 기호가 eIndexToDirect로 대체된다.
eIndexToDirect	: 이것은 FbxLayerElementTemplate::mIndexArray에 n 번째 요소에 대해
				  매핑 요소의 FbxLayerElementTemplate::mDirectArray 배열에 인덱스가 포함되어 있음을 나타낸다.
				  eIndexToDirect는 일반적으로 eByPolygonVertex 매핑 모드 요소 좌표를 저장하는 데 유용하다.
				  동일한 좌표는 일반적으로 여러 번 반복되기 때문에 좌표를 한 번만 저장하고 인덱스로 참조하여 공간을 절약한다.
				  머티리얼과 텍스처도이 모드로 참조되며 실제 머티리얼 / 텍스처는 FbxLayerElementTemplate::mDirectArray를 통해 액세스 할 수 있다.
*/
TMatrix TFbxImport::ParseTransform(FbxNode* fbxNode)
{
	/*TMatrix mat;
	FbxVector4 trans;
	if (fbxNode->LclTranslation.IsValid())
	{
		trans = fbxNode->LclTranslation.Get();
	}
	FbxVector4 rotation;
	if (fbxNode->LclRotation.IsValid())
	{
		trans = fbxNode->LclRotation.Get();
	}
	FbxVector4 Scale;
	if (fbxNode->LclScaling.IsValid())
	{
		trans = fbxNode->LclScaling.Get();
	}

	FbxMatrix matTransform(trans, rotation, Scale);
	mat = ConvertMatrix(matTransform);
	mat = DxConvertMatrix(mat);*/

	// EvaluateGlobalTransform = selfMat * parentMatrix;
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxTime time;
	time.SetFrame(0.0f, TimeMode);
	FbxAMatrix matFbx = fbxNode->EvaluateGlobalTransform(time);
	TMatrix mat = ConvertAMatrix(matFbx);
	mat = DxConvertMatrix(mat);
	return mat;
}
TMatrix TFbxImport::DxConvertMatrix(TMatrix& m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
TMatrix TFbxImport::ConvertMatrix(FbxMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
TMatrix TFbxImport::ConvertAMatrix(FbxAMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}

void      TFbxImport::PreProcess(FbxNode* fbxNode, TFbxObj* fbxobj)
{
	if (fbxNode == nullptr) return;
	if ( fbxNode->GetCamera() ||
		 fbxNode->GetLight() ) return;

	std::shared_ptr<TFbxObj> tFbxChildObj = std::make_shared<TFbxObj>();
	tFbxChildObj->m_szName = mtw(fbxNode->GetName());
	tFbxChildObj->m_iBone = TFbxObj::BONE_NODE;
	tFbxChildObj->m_iBoneIndex = fbxobj->m_TreeList.size();
	fbxobj->m_TreeList.push_back(tFbxChildObj);	
	fbxobj->m_pFbxModeNameList.push_back(tFbxChildObj->m_szName);
	fbxobj->m_pFbxNodeMap.insert(std::make_pair(tFbxChildObj->m_szName, m_dwNodeIndex));

	m_pFbxNodeList.push_back(fbxNode);
	m_pFbxNodeMap.insert(std::make_pair(fbxNode, m_dwNodeIndex));	
	m_dwNodeIndex++;

	FbxMesh* fbxMesh = fbxNode->GetMesh();
	if (fbxMesh != nullptr)
	{
		tFbxChildObj->m_iBone = TFbxObj::MESH_NODE;
		m_pFbxNodeMeshList.push_back(tFbxChildObj);
	}
	UINT iNumChild = fbxNode->GetChildCount();
	for (int iChild = 0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChildNode = fbxNode->GetChild(iChild);		
		PreProcess(pChildNode, fbxobj);
	}	
}

bool      TFbxImport::Load(T_STR filename, TFbxObj* fbxobj)
{
	C_STR name = wtm(filename);
	bool ret = m_pFbxImporter->Initialize(name.c_str());
	ret = m_pFbxImporter->Import(m_pFbxScene);
	
	FbxNode* m_FbxRootNode = m_pFbxScene->GetRootNode();
	if (m_FbxRootNode)
	{		
		PreProcess(m_FbxRootNode, fbxobj);
	}
	
	for (int iNode = 0; iNode < m_pFbxNodeMeshList.size(); iNode++)
	{
		std::shared_ptr<TFbxObj> fbxMesh = m_pFbxNodeMeshList[iNode];// std::make_shared<TFbxObj>();

		/*auto iter = m_pFbxNodeMap.find(m_pFbxNodeMeshList[iNode]);
		if (iter != m_pFbxNodeMap.end())
		{*/
			LoadMesh(m_pFbxNodeList[fbxMesh->m_iBoneIndex], *fbxMesh.get());
			//fbxMesh->m_szName = mtw(m_pFbxNodeMeshList[iNode]->GetName());
			//fbxMesh->m_matWorld = ParseTransform(m_pFbxNodeMeshList[iNode]);
			fbxobj->m_DrawList.push_back(fbxMesh);
		//}
		//else
		//{
		//	//error
		//}
	}

	GetAnimation(fbxobj);
	return true;
}
// vb, ib, worldmatrix, texture
void	  TFbxImport::LoadMesh(FbxNode* fbxNode, TFbxObj& tMesh)
{
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	UINT    iNumPolyCount = fbxMesh->GetPolygonCount();

	tMesh.m_bSkinning = ParseMeshSkinning(fbxMesh, &tMesh);
	tMesh.m_iBone = TFbxObj::SKIN_NODE;
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
		tMesh.m_pSubIWVertexList.resize(iNumMtrl);
	}
	else
	{
		tMesh.m_TriangleList.resize(1);
		tMesh.m_pSubIWVertexList.resize(1);
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

	// 1) polygon -> face 2개 인 경우 -> 폴리곤사이즈 4개 정점으로 구성.
	// 2) polygon -> face 1개 인 경우 -> 폴리곤사이즈 3개 정점으로 구성.
	//** 서브메터리얼 구조 일 경우는
	// 각 폴리곤 마다 해당하는 텍스처를 검색해서 별도로 저장해야 한다.
	UINT  iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly<iNumPolyCount; iPoly++)
	{
		// 서브메터리얼 검색
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

				// 가중치
				TVertexIW iwVertex;

				FbxGeometryElementTangent* vertexTangent = fbxMesh->GetElementTangent(0);
				/*if (vertexTangent != nullptr)
				{
					FbxVector4 tangent = ReadTangent(pFbxMesh,
						VertexTangentSet.size(),
						vertexTangent,
						DCCIndex,
						iBasePolyIndex + VertexIndex[iIndex]);
					iwVertex.tan.x = (FLOAT)tangent.mData[0];
					iwVertex.tan.y = (FLOAT)tangent.mData[2];
					iwVertex.tan.z = (FLOAT)tangent.mData[1];
				}
				else
				{*/
					iwVertex.tan = { 0,0,0 };
				//}


				if(tMesh.m_bSkinning)
				{
					TWeight* weight = &tMesh.m_WeightList[iDCIndex];
					for (int i = 0; i < 4; i++)
					{
						iwVertex.i[i] = weight->Index[i];
						iwVertex.w[i] = weight->Weight[i];
					}
				}
				else
				{
					// 일반오브젝트 에니메이션을 스키닝 케릭터 화 작업.
					iwVertex.i[0] = tMesh.m_iBoneIndex;
					iwVertex.w[0] = 1.0f;
				}
				tMesh.m_pSubIWVertexList[iSubMtrl].push_back(iwVertex);
			}
		}

		iBasePolyIndex += iNumPolySize;
	}
}
bool      TFbxImport::Init()
{
	m_pFbxModeNameList.clear();
	m_pFbxNodeMap.clear();
	m_pFbxNodeList.clear();
	m_pFbxNodeMeshList.clear();
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