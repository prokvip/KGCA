#define  _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "TFbxImporter.h"
namespace TBasisFBX
{
	TMatrix     TFbxImporter::DxConvertMatrix(TMatrix m)
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
	TMatrix     TFbxImporter::ConvertMatrix(FbxMatrix& m)
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
	TMatrix     TFbxImporter::ConvertAMatrix(FbxAMatrix& m)
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
	void		TFbxImporter::ParseAnimation()
	{
		FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
		FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
		if (stack == nullptr) return;

		FbxString TakeName = stack->GetName();
		FbxTakeInfo* TakeInfo = m_pFbxScene->GetTakeInfo(TakeName);
		FbxTimeSpan LocalTimeSpan = TakeInfo->mLocalTimeSpan;
		FbxTime start = LocalTimeSpan.GetStart();
		FbxTime end = LocalTimeSpan.GetStop();
		FbxTime Duration = LocalTimeSpan.GetDuration();

		FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
		FbxLongLong s = start.GetFrameCount(TimeMode);
		FbxLongLong n = end.GetFrameCount(TimeMode);
		m_Scene.iStart = s;
		m_Scene.iEnd = n;
		m_Scene.iFrameSpeed = 30;
		// 1�ʿ� 30 frame 
		// 1Frame = 160 Tick
		// 50 Frame 
		FbxTime time;
		TTrack tTrack;
		for (FbxLongLong t = s; t <= n; t++)
		{
			time.SetFrame(t, TimeMode);
			for (int iObj = 0; iObj < m_TreeList.size(); iObj++)
			{
				FbxAMatrix matGlobal = m_TreeList[iObj]->m_pFbxNode->EvaluateGlobalTransform(time);
				tTrack.iFrame = t;
				tTrack.matTrack = DxConvertMatrix(ConvertAMatrix(matGlobal));
				// ��ĺ���
				// ����� ����(SRT)
				TBasisFBX::D3DXMatrixDecompose(&tTrack.s, &tTrack.r, &tTrack.t, &tTrack.matTrack);
				m_TreeList[iObj]->m_AnimTrack.push_back(tTrack);
			}
		}
	}
	void    TFbxImporter::PreProcess(FbxNode* node, TFbxModel* fbxParent)
	{
		TFbxModel* fbx = nullptr;
		if (node != nullptr)
		{
			fbx = new TFbxModel;
			fbx->m_pFbxParent = node->GetParent();
			fbx->m_pFbxNode = node;
			fbx->m_csName = to_mw(node->GetName());
			fbx->m_pParentObj = fbxParent;
			fbx->m_iIndex = m_TreeList.size();
			m_TreeList.push_back(fbx);
			m_pFbxNodeMap.insert(std::make_pair(node, fbx->m_iIndex));
			m_pFbxModelMap.insert(std::make_pair(fbx->m_csName, fbx));
		}
		// camera, light, mesh, shape, animation
		FbxMesh* pMesh = node->GetMesh();
		if (pMesh)
		{
			m_DrawList.push_back(fbx);
		}
		int iNumChild = node->GetChildCount();
		for (int iNode = 0; iNode < iNumChild; iNode++)
		{
			FbxNode* child = node->GetChild(iNode);
			PreProcess(child, fbx);
		}
	}
	bool	TFbxImporter::Load(std::string filename)
	{
		m_pFbxManager = FbxManager::Create();
		m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
		m_pFbxScene = FbxScene::Create(m_pFbxManager, "");

		bool bRet = m_pFbxImporter->Initialize(filename.c_str());
		bRet = m_pFbxImporter->Import(m_pFbxScene);

		// Import ���� �۾��ؾ� �ݿ��ȴ�.
		FbxAxisSystem	 m_SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);
		FbxSystemUnit	m_SceneSystemUnit = m_pFbxScene->GetGlobalSettings().GetSystemUnit();

		if (m_SceneSystemUnit.GetScaleFactor() != 1)	//if (m_SceneSystemUnit != FbxSystemUnit::cm)
		{
			FbxSystemUnit::cm.ConvertScene(m_pFbxScene);
		}
		m_pRootNode = m_pFbxScene->GetRootNode();
		PreProcess(m_pRootNode, nullptr);
		ParseAnimation();
		for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
		{
			ParseMesh(m_DrawList[iObj]);
		}

		if (m_pFbxScene)m_pFbxScene->Destroy();
		if (m_pFbxImporter)m_pFbxImporter->Destroy();
		if (m_pFbxManager)m_pFbxManager->Destroy();
		m_pFbxScene = nullptr;
		m_pFbxImporter = nullptr;
		m_pFbxManager = nullptr;
		return true;
	}
	bool	TFbxImporter::ParseMeshSkinning(
		FbxMesh* pFbxMesh,
		TFbxModel* pObject)
	{
		int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (iDeformerCount == 0)
		{
			return false;
		}
		// ������ ������ �����Ѵ�.
		int iVertexCount = pFbxMesh->GetControlPointsCount();
		pObject->m_WeightList.resize(iVertexCount);

		for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
		{
			auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
			DWORD dwClusterCount = pSkin->GetClusterCount();
			// dwClusterCount�� ����� ��ü ������ ������ �־���.
			for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
			{
				auto pCluster = pSkin->GetCluster(dwClusterIndex);
				////
				FbxAMatrix matXBindPose;
				FbxAMatrix matReferenceGlobalInitPosition;
				pCluster->GetTransformLinkMatrix(matXBindPose);
				pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
				FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

				TMatrix matInvBindPos = DxConvertMatrix(ConvertMatrix(matBindPose));
				matInvBindPos = matInvBindPos.Invert();
				int  iBoneIndex = m_pFbxNodeMap.find(pCluster->GetLink())->second;
				std::wstring name = to_mw(pCluster->GetLink()->GetName());// m_TreeList[iBoneIndex]->m_csName;
				pObject->m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));

				int  dwClusterSize = pCluster->GetControlPointIndicesCount();
				// ������ �޴� �������� �ε���
				int* pIndices = pCluster->GetControlPointIndices();
				double* pWeights = pCluster->GetControlPointWeights();
				// iBoneIndex�� ������ �޴� �������� dwClusterSize�� �ִ�.
				for (int i = 0; i < dwClusterSize; i++)
				{
					// n�� ����(pIndices[i])�� iBoneIndex�� ��Ŀ� 
					// pWeights[i]�� ����ġ�� ����Ǿ���.
					int iVertexIndex = pIndices[i];
					float fWeight = pWeights[i];
					pObject->m_WeightList[iVertexIndex].InsertWeight(iBoneIndex, fWeight);
				}
			}
		}
		return true;
	}
	void	TFbxImporter::ParseMesh(TFbxModel* pObject)
	{
		FbxMesh* pFbxMesh = pObject->m_pFbxNode->GetMesh();

		pObject->m_bSkinned = ParseMeshSkinning(pFbxMesh, pObject);
		// �������(�ʱ� ���� ��ġ�� ��ȯ�� �� ���)
		FbxAMatrix geom;
		FbxVector4 trans = pObject->m_pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 rot = pObject->m_pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 scale = pObject->m_pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
		geom.SetT(trans);
		geom.SetR(rot);
		geom.SetS(scale);

		FbxAMatrix normalMatrix = geom;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();

		// ���̾� ( 1���� ������, �������� ���ļ� ������ ����)
		std::vector<FbxLayerElementUV*> VertexUVSet;
		std::vector<FbxLayerElementVertexColor*> VertexColorSet;
		std::vector<FbxLayerElementTangent*> VertexTangentSet;
		std::vector<FbxLayerElementMaterial*> MaterialSet;
		std::vector<FbxLayerElementNormal*>		VertexNormalSets;
		int iLayerCount = pFbxMesh->GetLayerCount();
		bool bFlag = false;

		if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetNormals() == nullptr)
		{
			pFbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
			pFbxMesh->GenerateNormals();
#else
			pFbxMesh->ComputeVertexNormals();
#endif
		}

		if (pFbxMesh->GetLayer(0)->GetTangents() == nullptr)
		{
			bFlag = pFbxMesh->GenerateTangentsData(0);
		}

		for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
		{
			FbxLayer* pFbxLayer = pFbxMesh->GetLayer(iLayer);
			if (pFbxLayer->GetUVs() != nullptr)
			{
				VertexUVSet.push_back(pFbxLayer->GetUVs());
			}
			if (pFbxLayer->GetVertexColors() != nullptr)
			{
				VertexColorSet.push_back(pFbxLayer->GetVertexColors());
			}
			if (pFbxLayer->GetTangents() != nullptr)
			{
				VertexTangentSet.push_back(pFbxLayer->GetTangents());
			}
			if (pFbxLayer->GetNormals() != NULL)
			{
				VertexNormalSets.push_back(pFbxLayer->GetNormals());
			}
			if (pFbxLayer->GetMaterials() != nullptr)
			{
				MaterialSet.push_back(pFbxLayer->GetMaterials());
			}
		}

		//  1���� ������Ʈ�� �������� �ؽ�ó�� ����Ѵ�.
		//  ������ �ؽ�ó�� �̸��� ��� �����Ѵ�.
		//  � ���̽�(������)�� � �ؽ�ó�� ����ϴ�?
		//  ���� �ؽ�ó�� ����ϴ� ������� ���� �����Ѵ�.
		int iNumMtrl = pObject->m_pFbxNode->GetMaterialCount();
		for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
		{
			FbxSurfaceMaterial* pSurface = pObject->m_pFbxNode->GetMaterial(iMtrl);
			if (pSurface)
			{
				std::string texturename = ParseMaterial(pSurface);
				std::wstring szTexFileName = L"../../data/fbx/";
				szTexFileName += to_mw(texturename);
				pObject->m_szTexFileList.push_back(szTexFileName);
				pObject->m_pTextureList.push_back(
					I_TextureFBX.Load(pObject->m_szTexFileList[iMtrl]));
			}
		}
		if (pObject->m_szTexFileList.size() > 0)
		{
			pObject->m_szTexFileName = pObject->m_szTexFileList[0];
		}
		if (iNumMtrl > 0)
		{
			pObject->m_pSubVertexList.resize(iNumMtrl);
			pObject->m_pSubIWVertexList.resize(iNumMtrl);
		}
		else
		{
			pObject->m_pSubVertexList.resize(1);
			pObject->m_pSubIWVertexList.resize(1);
		}

		int iBasePolyIndex = 0;
		// �ﰢ��, �簢��o
		int iNumPolyCount = pFbxMesh->GetPolygonCount();
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		int iNumFace = 0;
		for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
		{
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			iNumFace = iPolySize - 2;

			int iSubMtrl = 0;
			if (iNumMtrl >= 1 && MaterialSet[0] != nullptr)
			{
				iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet[0]);
			}
			for (int iFace = 0; iFace < iNumFace; iFace++)
			{
				// 1  2
				// 0  3
				// (max)021,032 ->  (dx)012, 023
				int VertexIndex[3] = { 0, iFace + 2, iFace + 1 };

				int CornerIndex[3];
				CornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				CornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 2);
				CornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 1);

				// uv
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 2);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 1);

				for (int iIndex = 0; iIndex < 3; iIndex++)
				{
					int DCCIndex = CornerIndex[iIndex];
					TVertex tVertex;
					// Max(x,z,y) ->(dx)x,y,z    		
					auto v = geom.MultT(pVertexPositions[DCCIndex]);
					tVertex.p.x = v.mData[0];
					tVertex.p.y = v.mData[2];
					tVertex.p.z = v.mData[1];


					if (VertexUVSet.size() > 0)
					{
						FbxLayerElementUV* pUVSet = VertexUVSet[0];
						FbxVector2 uv;
						ReadTextureCoord(
							pFbxMesh,
							pUVSet,
							DCCIndex,
							u[iIndex],
							uv);
						tVertex.t.x = uv.mData[0];
						tVertex.t.y = 1.0f - uv.mData[1];
					}

					FbxColor color = FbxColor(1, 1, 1, 1);
					if (VertexColorSet.size() > 0)
					{
						color = ReadColor(pFbxMesh,
							VertexColorSet.size(),
							VertexColorSet[0],
							DCCIndex,
							iBasePolyIndex + VertexIndex[iIndex]);
					}
					tVertex.c.x = color.mRed;
					tVertex.c.y = color.mGreen;
					tVertex.c.z = color.mBlue;
					tVertex.c.w = pObject->m_iIndex;

					if (VertexNormalSets.size() <= 0)
					{
						FbxVector4 normal = ReadNormal(pFbxMesh,
							DCCIndex,
							iBasePolyIndex + VertexIndex[iIndex]);
						normal = normalMatrix.MultT(normal);
						normal.Normalize();
						tVertex.n.x = normal.mData[0]; // x
						tVertex.n.y = normal.mData[2]; // z
						tVertex.n.z = normal.mData[1]; // y		
						D3DXVec3Normalize(&tVertex.n, &tVertex.n);
					}
					else
					{
						// Store vertex normal
						FbxVector4 finalNorm = ReadNormal(pFbxMesh, VertexNormalSets.size(),
							VertexNormalSets[0],
							DCCIndex,
							iBasePolyIndex + VertexIndex[iIndex]);// dwTriangleIndex * 3 + dwCornerIndex);
						finalNorm.mData[3] = 0.0;
						finalNorm = normalMatrix.MultT(finalNorm);
						finalNorm.Normalize();
						tVertex.n.x = finalNorm.mData[0];
						tVertex.n.y = finalNorm.mData[2];
						tVertex.n.z = finalNorm.mData[1];
						D3DXVec3Normalize(&tVertex.n, &tVertex.n);
					}

					// ����ġ
					TVertexIW iwVertex;

					FbxGeometryElementTangent* vertexTangent = pFbxMesh->GetElementTangent(0);
					if (vertexTangent != nullptr)
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
					{
						iwVertex.tan = { 0,0,0 };
					}


					if (pObject->m_bSkinned)
					{
						TWeight* weight = &pObject->m_WeightList[DCCIndex];
						for (int i = 0; i < 4; i++)
						{
							iwVertex.i[i] = weight->Index[i];
							iwVertex.w[i] = weight->Weight[i];
						}
					}
					else
					{
						// �Ϲݿ�����Ʈ ���ϸ��̼��� ��Ű�� �ɸ��� ȭ �۾�.
						iwVertex.i[0] = pObject->m_iIndex;
						iwVertex.w[0] = 1.0f;
					}
					//pObject->m_VertexList.push_back(tVertex);//36
					pObject->m_pSubVertexList[iSubMtrl].push_back(tVertex);
					pObject->m_pSubIWVertexList[iSubMtrl].push_back(iwVertex);
				}
			}

			iBasePolyIndex += iPolySize;
		}

	}
	bool	TFbxImporter::CreateConstantBuffer(ID3D11Device* pDevice)
	{
		HRESULT hr;
		//gpu�޸𸮿� ���� �Ҵ�(���ϴ� �Ҵ� ũ��)
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(TBoneWorld);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		//D3D11_SUBRESOURCE_DATA sd;
		//ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		//sd.pSysMem = &m_matBoneArray;

		if (FAILED(hr = pDevice->CreateBuffer(&bd, NULL,
			&m_pBoneCB)))
		{
			return false;
		}
		return true;
	}

	bool	TFbxImporter::Init()
	{
		m_pFbxManager = FbxManager::Create();
		m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
		m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
		return true;
	}
	bool	TFbxImporter::Frame()
	{
		return true;
	}
	bool	TFbxImporter::Render()
	{
		return true;
	}
	bool	TFbxImporter::Release()
	{
		if (m_pBoneCB)m_pBoneCB->Release();
		m_pBoneCB = nullptr;
		for (int iObj = 0; iObj < m_TreeList.size(); iObj++)
		{
			m_TreeList[iObj]->Release();
			delete m_TreeList[iObj];
		}
		m_pFbxModelMap.clear();
		m_TreeList.clear();
		m_DrawList.clear();
		if (m_pFbxScene)m_pFbxScene->Destroy();
		if (m_pFbxImporter)m_pFbxImporter->Destroy();
		if (m_pFbxManager)m_pFbxManager->Destroy();
		m_pFbxScene = nullptr;
		m_pFbxImporter = nullptr;
		m_pFbxManager = nullptr;
		return true;
	}

	bool	TFbxImporter::Load(ID3D11Device* pd3dDevice, std::wstring filename)
	{
		if (Load(to_wm(filename).c_str()))
		{
			CreateConstantBuffer(pd3dDevice);
			TShader* pVShader = I_ShaderFBX.CreateVertexShader(pd3dDevice, L"Character.hlsl", "VS");
			TShader* pPShader = I_ShaderFBX.CreatePixelShader(pd3dDevice, L"Character.hlsl", "PS");
			for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
			{
				m_DrawList[iObj]->Init();
				m_DrawList[iObj]->m_pVShader = pVShader;
				m_DrawList[iObj]->m_pPShader = pPShader;
				if (!m_DrawList[iObj]->Create(pd3dDevice, m_pContext))
				{
					return false;
				}
				m_DrawList[iObj]->GenAABB();
			}
		}
		return true;
	}
};