#define _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"

bool TFbxObj::Load(ID3D11Device* pd3dDevice,
	const char* szLoadName,
	const TCHAR* pLoadShaderFile, bool bThread)
{
	m_dxobj.m_iVertexSize = sizeof(PNCT3_VERTEX);
	m_dxobj.m_iIndexSize = sizeof(DWORD);
	if (!LoadFBX(szLoadName))
	{
		return false;
	}
	if (!Convert(pd3dDevice))
	{
		return false;
	}
	m_dxobj.m_iNumVertex = m_iMaxVertex;
	m_dxobj.m_iNumIndex = m_iMaxIndex;

	if (!bThread && !Create(pd3dDevice, pLoadShaderFile))
	{
		return false;
	}
	return true;
}
void TFbxObj::SetBindPose()
{
	assert(m_pFBXScene != nullptr);
	string printName;
	m_BindPoses.clear();
	INT iPoseCount = m_pFBXScene->GetPoseCount();
	for (INT i = 0; i < iPoseCount; ++i)
	{
		auto pPose = m_pFBXScene->GetPose(i);
		INT iNodeCount = pPose->GetCount();

		for (INT j = 0; j < iNodeCount; ++j)
		{
			auto pPoseNode = pPose->GetNode(j);
			printName = pPoseNode->GetName();
		}
		if (pPose->IsBindPose())
		{
			m_BindPoses.push_back(pPose);
		}
	}
	if (m_BindPoses.empty())
	{
		//if (g_pScene->Settings().bExportAnimations)
		{
			//ExportLog::LogWarning("No valid bind pose found; will export scene using the default pose.");
		}
		return;
	}
	auto pAnimEvaluator = m_pFBXScene->GetAnimationEvaluator();

	size_t dwPoseCount = m_BindPoses.size();
	for (size_t i = 0; i < dwPoseCount; ++i)
	{
		FbxPose* pPose = m_BindPoses[i];
		INT iNodeCount = pPose->GetCount();
		for (INT j = 0; j < iNodeCount; ++j)
		{
			auto pNode = pPose->GetNode(j);
			auto matNode = pPose->GetMatrix(j);

			PoseMap::iterator iter = m_BindPoseMap.find(pNode);
			if (iter != m_BindPoseMap.end())
			{
				FbxMatrix matExisting = iter->second;
				if (matExisting != matNode)
				{
					printName = pNode->GetName();
				}
			}

			m_BindPoseMap[pNode] = matNode;

			auto matGlobal = pAnimEvaluator->GetNodeGlobalTransform(pNode, 0);
			
			
			////// 정점을 바이패트 공간에서 월드 공간으로 변환한다.
			/*TMatrix matDxGlobal = DxConvertMatrix(ConvertMatrixA(matGlobal));
			TMatrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matNode));
			D3DXMatrixInverse(&matInvBindPos, NULL, &matInvBindPos);
			*/
		}
	}

	//ExportLog::LogMsg(3, "Created bind pose map with %Iu nodes.", m_BindPoseMap.size());
}
void TFbxObj::PreProcess(FbxNode* pNode)
{
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}		
	//if (pNode->GetMaterialCount() > 0)
	//{
	//	int iNumFbxMaterial = pNode->GetMaterialCount();		
	//	for (int iMtrl = 0; iMtrl < iNumFbxMaterial; iMtrl++)
	//	{
	//		m_pRootMtrlList.push_back(pNode->GetMaterial(iMtrl));
	//	}
	//}

	TMatrix mat;
	D3DXMatrixIdentity(&mat);
	DxMatrixMap::iterator iter = m_matFrameMatrixMap.find(mtw(pNode->GetName()));
	if (iter == m_matFrameMatrixMap.end())
	{
		m_matFrameMatrixMap[mtw(pNode->GetName())] = mat;
	}
	DWORD dwChildCount = pNode->GetChildCount();
	for (DWORD i = 0; i < dwChildCount; ++i)
	{
		FbxNode* pFbxChildNode = pNode->GetChild(i);
		if (pFbxChildNode->GetNodeAttribute() != NULL)
		{
			//FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
			//if (AttributeType == FbxNodeAttribute::eLODGroup)
			//{
			//	std::string name = pNode->GetName();
			//	//continue;
			//}
			//if (AttributeType != FbxNodeAttribute::eMesh &&
			//	AttributeType != FbxNodeAttribute::eSkeleton&&
			//	AttributeType != FbxNodeAttribute::eNull)
			//{
			//	continue;
			//}
		}
		PreProcess(pFbxChildNode);
	}
}
bool TFbxObj::LoadFBX(const char* pFileName)
{
	if (Initialize(pFileName) != S_OK)
	{
		return false;
	}
	//SetBindPose();
	m_bBindPoseFixupRequired = false;

	assert(m_pFBXScene->GetRootNode() != NULL);
	FbxNode* pFbxRootNode = m_pFBXScene->GetRootNode();
	assert(m_pFBXScene->GetRootNode() != NULL);

	TMatrix matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	PreProcess(pFbxRootNode);
	// 모든 노드르 순회하여 메쉬정보 및 에니메이션 정보를 저장한다.	
	ParseNode(pFbxRootNode, m_pFBXScene, NULL, matIdentity);

	//if (m_bBindPoseFixupRequired)
	//{
	//	//ExportLog::LogMsg(2, "Fixing up frames with updated bind pose.");
	//	FixupNode(m_pFBXScene, matIdentity);
	//}
	ParseAnimation(m_pFBXScene);
	return true;
}
TMatrix TFbxObj::ParseTransform(FbxNode* pNode,const TMatrix& matParentWorld,const BOOL bWarnings)
{
	TMatrix matWorld;
	TMatrix matLocal;
	TMatrix matGeom;
	BOOL bProcessDefaultTransform = TRUE;

	FbxMatrix geom = GetGeometryTransformation(pNode);

	if (!m_BindPoseMap.empty())
	{
		PoseMap::iterator iter = m_BindPoseMap.find(pNode);
		if (iter != m_BindPoseMap.end())
		{
			FbxMatrix PoseMatrix = iter->second;
			matWorld = DxConvertMatrix(ConvertMatrixA(PoseMatrix));
			matWorld = DxConvertMatrix(ConvertMatrixA(geom));
			bProcessDefaultTransform = FALSE;
		}
	}

	if (bProcessDefaultTransform)
	{
		FbxVector4 Translation;
		if (pNode->LclTranslation.IsValid())
			Translation = pNode->LclTranslation.Get();

		FbxVector4 Rotation;
		if (pNode->LclRotation.IsValid())
			Rotation = pNode->LclRotation.Get();

		FbxVector4 Scale;
		if (pNode->LclScaling.IsValid())
			Scale = pNode->LclScaling.Get();

		FbxMatrix matTransform(Translation, Rotation, Scale);
		matGeom = DxConvertMatrix(ConvertMatrixA(geom));
		matLocal = DxConvertMatrix(ConvertMatrixA(matTransform));
		matWorld =  matLocal * matParentWorld;
	}
	return matWorld;
}
VOID TFbxObj::ClearScene()
{
	m_pFBXScene->Clear();
}
VOID TFbxObj::ParseNode(FbxNode* pNode, FbxScene* pScene, TMesh* pParent, const TMatrix& matParentWorld )
{
	if (pNode == nullptr)
	{
		return;
	}
	if (pNode->GetCamera() || pNode->GetLight() )
	{
		return;
	}	
	shared_ptr<tFbxData> pData = make_shared<tFbxData>();
	shared_ptr<TMesh> pMesh = make_shared<TMesh>();
	pMesh->m_szName = mtw(pNode->GetName());	
	// 사용처를 생략했다.(주석도 무방하나 디버그를 위해서 남겨둠)
	TMatrix matWorld = ParseTransform(pNode, matParentWorld);

	// m_matXFormToWorld도 정점에 적용해두었기 때문에 생략가능함.
	FbxMatrix geom = GetGeometryTransformation(pNode);
	TMatrix matGeom = DxConvertMatrix(ConvertMatrixA(geom));
	pMesh->m_matXFormToWorld = matGeom;	

	pMesh->m_pParent = pParent;
	pMesh->m_iMtrlRef = -1;

	pData->m_iNumFace = 0;
	pMesh->m_iNumFace = 0;

	m_pMesh.push_back(pMesh);
	m_pData.push_back(pData);

	m_tMeshMap[pNode] = pMesh.get();

	if (pNode->GetMesh() != NULL)
	{
		pMesh->m_ClassType = CLASS_GEOM;
		ParseMesh(pNode, pNode->GetMesh(), pMesh.get(), pData.get(), FALSE);
	}
	else
	{
		pMesh->m_ClassType = CLASS_BONE;		
	}

	DWORD dwChildCount = pNode->GetChildCount();
	for (DWORD i = 0; i < dwChildCount; ++i)
	{
		FbxNode* pFbxChildNode = pNode->GetChild(i);
		if (pFbxChildNode->GetNodeAttribute() != NULL)
		{
			std::string name;
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
			if (AttributeType == FbxNodeAttribute::eLODGroup)
			{
				name = pNode->GetName();
			}
		}
		ParseNode(pFbxChildNode, pScene, pMesh.get(), matWorld );
	}
}
bool TFbxObj::ParseMeshSkinning(FbxMesh* pMesh, SkinData* pSkinData)
{
	// 스킨 모디파이어 영향을 받았으면
	DWORD dwDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (dwDeformerCount == 0)
		return false;
	// 영향을 받은 제어점의 개수
	const DWORD dwVertexCount = pMesh->GetControlPointsCount();
	// 정점 당 8 개까지 가중치값 저장하겠다.
	const DWORD dwStride = 8;
	pSkinData->Alloc(dwVertexCount, dwStride);

	for (DWORD dwDeformerIndex = 0; dwDeformerIndex < dwDeformerCount; ++dwDeformerIndex)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		// 영향을 미치는 행렬이 dwClusterCount 있다.
		for (DWORD dwClusterIndex = 0; dwClusterIndex < dwClusterCount; ++dwClusterIndex)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			// 영향을 미치는 행렬이 dwClusterSize 정점에 영향을 미친다.
			DWORD dwClusterSize = pCluster->GetControlPointIndicesCount();
			//if (dwClusterSize == 0)
			//	continue;

			auto pLink = pCluster->GetLink();

			DWORD dwBoneIndex = pSkinData->GetBoneCount();
			pSkinData->InfluenceNodes.push_back(pLink);

			FbxAMatrix matXBindPose;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
#ifdef _DEBUG
			//lSkinningType == FbxSkin::eRigid
			FbxSkin::EType lSkinningType = pSkin->GetSkinningType();
			//lClusterMode == FbxCluster::eNormalize
			FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();
#endif
			
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() *matXBindPose;
			CaptureBindPoseMatrix( pSkinData, pLink, matBindPose);

			// dwBoneIndex노드가 영향을 미치는 정점인덱스 배열
			INT* pIndices = pCluster->GetControlPointIndices();
			// dwBoneIndex노드가 영향을 미치는 정점가중치 배열
			DOUBLE* pWeights = pCluster->GetControlPointWeights();
			
			for (DWORD i = 0; i < dwClusterSize; ++i)
			{
				//pIndices[i]정점은 dwBoneIndex 행렬이 pWeights[i] 가중치로 영향을 받는다.
				pSkinData->InsertWeight(pIndices[i], dwBoneIndex, (float)pWeights[i]);
			}
		}
	}

	return true;
}
void TFbxObj::CaptureBindPoseMatrix( SkinData* pSkinData, FbxNode* pNode, const FbxMatrix& matBindPose)
{	
	TMatrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matBindPose));
	D3DXMatrixInverse(&matInvBindPos, NULL, &matInvBindPos);
	pSkinData->m_matBindPoseMap[mtw(pNode->GetName())] = matInvBindPos;
}
void TFbxObj::ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, TMesh* pMesh, tFbxData* pData, BOOL bSubDProcess, const CHAR* strSuffix)
{
	//FILE* fp = fopen("aa.txt", "wt");
	std::vector<TFbxMaterial*> tMaterial;

	if (pFbxMesh)
	{		
		//Using multiple layers
		//https://download.autodesk.com/us/fbx/20102/fbx_sdk_help/index.html?url=WS73099cc142f487551fea285e1221e4f9ff8-3bde.htm,topicNumber=d0e6331
		DWORD dwLayerCount = pFbxMesh->GetLayerCount();		
		bool bFlag = false;

		if (dwLayerCount == 0 || pFbxMesh->GetLayer(0)->GetNormals() == nullptr)
		{
			pFbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
			pFbxMesh->GenerateNormals();
#else
			pFbxMesh->ComputeVertexNormals();
#endif
		}
		// 접선벡터 계산
		if (pFbxMesh->GetLayer(0)->GetTangents() == nullptr)
		{
			bFlag = pFbxMesh->GenerateTangentsData(0);
		}
	
		std::vector<FbxLayerElementUV*>				VertexUVSets;
		std::vector<FbxLayerElementMaterial*>		VertexMaterialSets;
		std::vector<FbxLayerElementVertexColor*>	VertexColorSets;
		std::vector<FbxLayerElementNormal*>			VertexNormalSets;
		std::vector<FbxLayerElementTangent*>		VertexTangentSets;
		// 정점에 다수의 텍스처가 서로 다른 텍스처 좌표(pLayer->GetUVs())로 매핑(pLayer->GetMaterials()) 되어 있을 경우에 해당한다.
		// 정점 위치 제외한 정점노말, 정점접선벡터, 정점컬러, 정점텍스처 좌표 등이 레이어 별로 세팅될 수 있다.		 

		TLayerInfo tLayerInfo;
		tLayerInfo.pFbxMesh = pFbxMesh;
		for (DWORD dwLayerIndex = 0; dwLayerIndex < dwLayerCount; ++dwLayerIndex)
		{			
			FbxLayer* pLayer = pFbxMesh->GetLayer(dwLayerIndex);
			TLayer  tLayer;
			if (pLayer->GetVertexColors() != NULL)
			{
				VertexColorSets.push_back(pLayer->GetVertexColors());
				tLayer.VertexColorSets.push_back(pLayer->GetVertexColors());
			}
			if (pLayer->GetUVs() != NULL)
			{
				VertexUVSets.push_back(pLayer->GetUVs());
				tLayer.VertexUVSets.push_back(pLayer->GetUVs());
			}
			if (pLayer->GetNormals() != NULL)
			{
				VertexNormalSets.push_back(pLayer->GetNormals());
				tLayer.VertexNormalSets.push_back(pLayer->GetNormals());
			}
			if (pLayer->GetTangents() != NULL)
			{
				VertexTangentSets.push_back(pLayer->GetTangents());
				tLayer.VertexTangentSets.push_back(pLayer->GetTangents());
			}
			if (pLayer->GetMaterials() != NULL )
			{
				// 2개 이상의 레이어는 사용을 하지 않는다. 루트 매터리얼이 적용된 것이 2개 이상일 경우가 된다.
				// 이 경우는 기존 메터리얼은 무시하도록 할 것이다.
				FbxLayerElementMaterial* pMaterial = pLayer->GetMaterials();
				VertexMaterialSets.push_back(pMaterial);
				tLayer.VertexMaterialSets.push_back(pMaterial);

				int dwMaterialCount = pMaterial->mDirectArray->GetCount();					
				auto b = pMaterial->mDirectArray->GetAt(0);
				FbxLayerElementArrayTemplate<int>* a= pMaterial->mIndexArray;// ->GetCounter();
				int kk = a->GetCount();
				if (dwMaterialCount > 1)
				{
					for (DWORD i = 0; i < dwMaterialCount; ++i)
					{
						TFbxMaterial* pMaterial = ParseMaterialInLayer(pFbxMesh, pLayer, i);
						tMaterial.push_back(pMaterial);

						auto pSubData = make_shared<tFbxData>();
						auto pSubMesh = make_shared<TMesh>();

						pMesh->m_pSubMesh.push_back(pSubMesh);
						pData->m_pSubMesh.push_back(pSubData);
					}
				}
				else
				{
					TFbxMaterial* pMaterial = ParseMaterialInLayer(pFbxMesh, pLayer, 0);
					tMaterial.push_back(pMaterial);
				}
			}

			tLayer.pLayer = pLayer;
			tLayerInfo.tLayerList.push_back(tLayer);
		}
		m_pFbxLayerList.push_back(tLayerInfo);

		//int iNumMtrl = pNode->GetMaterialCount();
		//for (int dwMaterial = 0; dwMaterial < iNumMtrl; ++dwMaterial)
		//{
		//	auto pMat = pNode->GetMaterial(dwMaterial);
		//	if (!pMat)
		//		continue;

		//	auto pMaterial = ParseMaterial(pMat);
		//	//fbxMaterialList.push_back(pMaterial);
		//}
		//// 서브메터리얼을 얻고 메쉬를 해당 메터리얼로 구분한다.
		//DWORD dwMaterialCount = fbxMaterialList.size();
		//if (dwMaterialCount > 1)
		//{
		//	for (DWORD i = 0; i < dwMaterialCount; ++i)
		//	{
		//		auto pSubData = make_shared<tFbxData>();
		//		auto pSubMesh = make_shared<TMesh>();
		//		pMesh->m_pSubMesh.push_back(pSubMesh);
		//		pData->m_pSubMesh.push_back(pSubData);
		//	}
		//}

		pMesh->m_iMtrlRef = m_pMaterialList.size();
		m_pMaterialList.push_back(tMaterial);


		DWORD dwPolyCount = pFbxMesh->GetPolygonCount();
		DWORD dwVertexCount = pFbxMesh->GetControlPointsCount();
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();

		SkinData skindata;
		bool bSkinnedMesh = ParseMeshSkinning(pFbxMesh, &skindata);
		if (bSkinnedMesh)
		{
			DWORD dwBoneCount = skindata.GetBoneCount();
			for (DWORD i = 0; i < dwBoneCount; ++i)
			{
				T_STR name = mtw(skindata.InfluenceNodes[i]->GetName());
				pMesh->AddInfluence(name, skindata.m_matBindPoseMap[name]);
			}
		}
		else
		{
			TMatrix mat;
			D3DXMatrixIdentity(&mat);
			pMesh->AddInfluence(pMesh->m_szName, mat);
		}

		// pNode->EvaluateGlobalTransform(1.0f) : ASE 월드행렬
		FbxAMatrix vertMatrix;
		FbxAMatrix normMatrix;
		{
			// 월드 좌표를 로칼 좌표로 변환하는행렬( 월드행렬의 역행렬 )
			auto trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			auto rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			auto scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

			FbxAMatrix geom;
			geom.SetT(trans);
			geom.SetR(rot);
			geom.SetS(scale);

			vertMatrix = geom;

			// Calculate the normal transform matrix (inverse-transpose)
			normMatrix = vertMatrix;
			normMatrix = normMatrix.Inverse();
			normMatrix = normMatrix.Transpose();
		}
		DWORD basePolyIndex = 0;
		DWORD dwVertexCounter = 0;
		for (DWORD dwPolyIndex = 0; dwPolyIndex < dwPolyCount; ++dwPolyIndex)
		{
			DWORD dwPolySize = pFbxMesh->GetPolygonSize(dwPolyIndex);
			assert(dwPolySize >= 3);
			DWORD dwTriangleCount = dwPolySize - 2;
			assert(dwTriangleCount > 0);


			// 서브매터리얼 인덱스 
			// 주의 ---> 0번 레이어만 사용 할 것이다.
			DWORD dwMaterialIndex = 0;
			if (VertexMaterialSets.size())
			{
				if (VertexMaterialSets[0] != NULL) 
				{
					switch (VertexMaterialSets[0]->GetMappingMode())
					{
					case FbxLayerElement::eByPolygon:
						switch (VertexMaterialSets[0]->GetReferenceMode())
						{
						case FbxLayerElement::eDirect:
							dwMaterialIndex = dwPolyIndex;
							break;
						case FbxLayerElement::eIndex:
						case FbxLayerElement::eIndexToDirect:
							dwMaterialIndex = VertexMaterialSets[0]->GetIndexArray().GetAt(dwPolyIndex);
							//dwMaterialIndex = VertexMaterialSets[0]->GetIndexArray()[dwPolyIndex];
							break;
						}
					case FbxLayerElement::eAllSame:
						break;
					}
				}
			}

			DWORD dwCornerIndices[3];
			for (DWORD dwTriangleIndex = 0; dwTriangleIndex < dwTriangleCount; ++dwTriangleIndex)
			{
				dwCornerIndices[0] = pFbxMesh->GetPolygonVertex(dwPolyIndex, 0);
				dwCornerIndices[1] = pFbxMesh->GetPolygonVertex(dwPolyIndex, dwTriangleIndex + 2);
				dwCornerIndices[2] = pFbxMesh->GetPolygonVertex(dwPolyIndex, dwTriangleIndex + 1);

				INT iPolyIndex = (INT)dwPolyIndex;
				INT iVertIndex[3] = { 0, (INT)dwTriangleIndex + 2, (INT)dwTriangleIndex + 1 };

				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(dwPolyIndex, 0);
				u[1] = pFbxMesh->GetTextureUVIndex(dwPolyIndex, dwTriangleIndex + 2);
				u[2] = pFbxMesh->GetTextureUVIndex(dwPolyIndex, dwTriangleIndex + 1);

				TTri<PNCT3_VERTEX> tri;
				tri.iSubIndex = dwMaterialIndex;

				for (DWORD dwCornerIndex = 0; dwCornerIndex < 3; ++dwCornerIndex)
				{
					//int controlPointIndex = pFbxMesh->GetPolygonVertex(dwPolyIndex, dwCornerIndex);
					const DWORD& dwDCCIndex = dwCornerIndices[dwCornerIndex];
					auto finalPos = vertMatrix.MultT(pVertexPositions[dwDCCIndex]);
					PNCT3_VERTEX vertex;
					ZeroMemory(&vertex, sizeof(PNCT3_VERTEX));
					vertex.p.x = (float)finalPos.mData[0];
					vertex.p.y = (float)finalPos.mData[2];
					vertex.p.z = (float)finalPos.mData[1];

					// Store vertex normal
					FbxVector4 finalNorm = ReadNormal(pFbxMesh, VertexNormalSets.size(), 
						VertexNormalSets[0], 
						dwDCCIndex, basePolyIndex + iVertIndex[dwCornerIndex]);// dwTriangleIndex * 3 + dwCornerIndex);
					finalNorm.mData[3] = 0.0;
					finalNorm = normMatrix.MultT(finalNorm);
					finalNorm.Normalize();
					vertex.n.x = finalNorm.mData[0];
					vertex.n.y = finalNorm.mData[2];
					vertex.n.z = finalNorm.mData[1];
					D3DXVec3Normalize(&vertex.n, &vertex.n);

					if (VertexUVSets.size())
					{				
						FbxLayerElementUV* pUVSet = VertexUVSets[0];
						FbxVector2 Value(0, 0);
						Value = ReadTextureCoord(pFbxMesh, VertexUVSets.size(), pUVSet, dwDCCIndex, u[dwCornerIndex]);
						vertex.t.x = (FLOAT)Value.mData[0];
						vertex.t.y = 1.0f - (FLOAT)Value.mData[1];
					}

					vertex.c.x = 1;
					vertex.c.y = 1;
					vertex.c.z = 1;
					vertex.c.w = 1;
					// 정점텍스처좌표=1 이기 때문에 정점컬러를 사용하였기 때문에 2개 이상 정점컬러가 있으면 1개만 사용하였다.
					if (VertexColorSets.size())
					{
						// Store vertex color set
						FbxColor Value = ReadColor(pFbxMesh, VertexColorSets.size(), VertexColorSets[0],
							dwDCCIndex, basePolyIndex + iVertIndex[dwCornerIndex]);
						vertex.c.x = (FLOAT)Value.mRed;
						vertex.c.y = (FLOAT)Value.mGreen;
						vertex.c.z = (FLOAT)Value.mBlue;
						vertex.c.w = 1;// (FLOAT)Value.mAlpha;
					}
					//// Store vertex tangent set
					//FbxGeometryElementTangent* vertexTangent = pFbxMesh->GetElementTangent(0);
					//if (vertexTangent != nullptr)
					//{					
					//	FbxVector4 tangent = ReadTangent(pFbxMesh, VertexTangentSets.size(), 
					//		vertexTangent,
					//		dwDCCIndex, basePolyIndex + iVertIndex[dwCornerIndex]);		
					//	vertex.tan.x = (FLOAT)tangent.mData[0];
					//	vertex.tan.y = (FLOAT)tangent.mData[2];
					//	vertex.tan.z = (FLOAT)tangent.mData[1];
					//}
					//else
					//{
					//	vertex.tan = { 0,0,0 };
					//}

					// 가중치 및 인덱스 저장
					if (bSkinnedMesh)
					{
						int* pPoint = skindata.GetIndices(dwDCCIndex);
						vertex.i.x = pPoint[0];
						vertex.i.y = pPoint[1];
						vertex.i.z = pPoint[2];
						vertex.i.w = pPoint[3];
						//memcpy(&vertex.i, pPoint, sizeof(int)*4);
						float* fPoint = skindata.GetWeights(dwDCCIndex);
						vertex.w.x = fPoint[0];
						vertex.w.y = fPoint[1];
						vertex.w.z = fPoint[2];
						vertex.w.w = fPoint[3];
						//memcpy(&vertex.w, skindata.GetWeights(dwDCCIndex), sizeof(TVector4));
						//fprintf(fp, "\n%ld:%10.4f,%10.4f,%10.4f,%10.4f", dwDCCIndex, vertex.i[0], vertex.i[1], vertex.i[2], vertex.i[3]);
						//fprintf(fp, "  %ld:%10.4f,%10.4f,%10.4f,%10.4f", dwDCCIndex, vertex.w[0], vertex.w[1], vertex.w[2], vertex.w[3]);
					}
					else
					{
						vertex.i.x = 0; // 자기 자신
						vertex.w.x = 1.0f;
					}
					tri.vVertex[dwCornerIndex] = vertex;
				}


				if (pData->m_pSubMesh.size() > 0)
				{
					pData->m_pSubMesh[dwMaterialIndex]->m_TriList.push_back(tri);
					pData->m_pSubMesh[dwMaterialIndex]->m_iNumFace++;
				}
				else
				{
					pData->m_TriList.push_back(tri);
				}
				pData->m_iNumFace++;
			}
			basePolyIndex += dwPolySize;
		}
		//fclose(fp);
	}
}