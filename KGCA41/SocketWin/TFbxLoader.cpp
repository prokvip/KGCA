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
	// ����
	FbxSystemUnit::m.ConvertScene(m_pFbxScene);
	// ����(���)	
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);
	
	//FbxGeometryConverter converter(m_pFbxManager);
	//converter.Triangulate(m_pFbxScene, true);

	m_pRootNode = m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode);

	for (auto tObj : m_pObjectList)
	{
		if (tObj->m_pFbxParentNode != nullptr)
		{
			auto data = m_pObjectMap.find(tObj->m_pFbxParentNode);
			tObj->SetParent(data->second);
		}
		LoadAnimation(tObj);
		FbxMesh* pFbxMesh = tObj->m_pFbxNode->GetMesh();
		if (pFbxMesh)
		{
			m_pFbxMeshList.push_back(pFbxMesh);
			ParseMesh(pFbxMesh, tObj);
		}		
	}
	return true;
}
void TFbxLoader::LoadAnimation(TFbxObject* pObj)
{
	FbxNode* pNode = pObj->m_pFbxNode;
	FbxAnimStack* stackAnim = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	FbxLongLong s = 0;
	FbxLongLong n = 0;
	FbxTime::EMode TimeMode;
	if (stackAnim)
	{
		FbxString takeName = stackAnim->GetName();
		FbxTakeInfo* take = m_pFbxScene->GetTakeInfo(takeName);
		FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
		TimeMode = FbxTime::GetGlobalTimeMode();
		FbxTimeSpan localTimeSpan = take->mLocalTimeSpan;
		FbxTime start = localTimeSpan.GetStart();
		FbxTime end = localTimeSpan.GetStop();
		FbxTime Duration = localTimeSpan.GetDirection();
		s = start.GetFrameCount(TimeMode);
		n = end.GetFrameCount(TimeMode);
	}
	pObj->m_AnimScene.iStartFrame = s;
	pObj->m_AnimScene.iEndFrame = n;
	pObj->m_AnimScene.fFrameSpeed = 30.0f;
	pObj->m_AnimScene.fTickPerFrame = 160;
	FbxTime time;	
	for (FbxLongLong t = s; t <= n; t++)
	{
		time.SetFrame(t, TimeMode);
		TAnimTrack track;
		track.iFrame = t;
		FbxAMatrix fbxMatrix = pNode->EvaluateGlobalTransform(time);	
		//track.fbxMatrix = fbxMatrix;
		track.matAnim = DxConvertMatrix(fbxMatrix);		
		TBASIS_EX::D3DXMatrixDecompose(&track.s, &track.r, &track.t, &track.matAnim);
		pObj->m_AnimTracks.push_back(track);
		
	}
}
TMatrix TFbxLoader::ConvertMatrix(FbxAMatrix& fbxMatrix)
{
	TMatrix mat;
	float* tArray = (float*)(&mat);
	double* fbxArray = (double*)(&fbxMatrix);
	for (int i = 0; i < 16; i++)
	{
		tArray[i] = fbxArray[i];
	}
	return mat;
}
TMatrix TFbxLoader::DxConvertMatrix(FbxAMatrix& fbxMatrix)
{
	TMatrix m = ConvertMatrix(fbxMatrix);
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
void TFbxLoader::ParseMesh(FbxMesh* pFbxMesh, TFbxObject* pObject)
{
	FbxNode* pNode = pFbxMesh->GetNode();	
	FbxAMatrix geom; // ����(��Į)���(�ʱ� ���� ��ġ�� ��ȯ�� �� ����Ѵ�.)
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);
	FbxAMatrix normalLocalMatrix = geom;
	normalLocalMatrix = normalLocalMatrix.Inverse();
	normalLocalMatrix = normalLocalMatrix.Transpose();

	//FbxAMatrix matWorldTransform= pObject->m_fbxLocalMatrix;
	////// ����������� = �ڱ�(���ϸ��̼�) ��� * �θ�((���ϸ��̼�))���
	////if (pObject->m_pParent)
	////{
	////	matWorldTransform = 
	////		pObject->m_pParent->m_fbxLocalMatrix * 
	////		pObject->m_fbxLocalMatrix;
	////}		
	//FbxAMatrix normalWorldMatrix = matWorldTransform;
	//normalWorldMatrix = normalWorldMatrix.Inverse();
	//normalWorldMatrix = normalWorldMatrix.Transpose();

	// Layer ����
	FbxLayerElementUV* VertexUVSet=nullptr;
	FbxLayerElementVertexColor* VertexColorSet = nullptr;
	FbxLayerElementNormal* VertexNormalSet = nullptr;
	FbxLayerElementMaterial* MaterialSet = nullptr;
	FbxLayer* pFbxLayer  = pFbxMesh->GetLayer(0);
	if (pFbxLayer->GetUVs() != nullptr)
	{
		VertexUVSet=pFbxLayer->GetUVs();
	}
	if (pFbxLayer->GetVertexColors() != nullptr)
	{
		VertexColorSet = pFbxLayer->GetVertexColors();
	}
	if (pFbxLayer->GetNormals() != nullptr)
	{
		VertexNormalSet = pFbxLayer->GetNormals();
	}
	if (pFbxLayer->GetMaterials() != nullptr)
	{
		MaterialSet = pFbxLayer->GetMaterials();
	}

	std::string szFileName;
	int iNumMtrl = pNode->GetMaterialCount();
	std::vector< C_STR>   texFullNameList;
	texFullNameList.resize(iNumMtrl);
	
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		// 24 �̻��� ������ �ִ�.
		FbxSurfaceMaterial* pSurface = pNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (property.IsValid())
			{
				const FbxFileTexture* tex = property.GetSrcObject<FbxFileTexture>(0);
				if (tex)
				{
					szFileName = tex->GetFileName();
					texFullNameList[iMtrl] = szFileName;
				}
			}
		}
	}

	if (iNumMtrl == 1)
	{
		pObject->m_szTextureName = I_Tex.GetSplitName(szFileName);
	}
	if (iNumMtrl > 1)
	{
		pObject->vbDataList.resize(iNumMtrl);
		pObject->vbTexList.resize(iNumMtrl);
		for (int iTex = 0; iTex < iNumMtrl; iTex++)
		{
			pObject->vbTexList[iTex] = I_Tex.GetSplitName(
				texFullNameList[iTex]);
		}
	}

	int iNumPolyCount = pFbxMesh->GetPolygonCount();
	// 3 ���� -> 1������( �ﰢ��)
	// 4 ���� -> 1������( ���� )
	int iNumFace = 0;
	int iBasePolyIndex = 0;
	int iSubMtrl = 0;
	// ������
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iNumFace = iPolySize - 2;
		if (MaterialSet)
		{
			iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet);
		}
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// �����÷��ε���
			int VertexColor[3] = { 0, iFace + 2, iFace + 1 };
			// �����ε���
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
				FbxVector4 v2 = pVertexPositions[vertexID];
				PNCT_VERTEX tVertex; 
				FbxVector4 v = geom.MultT(v2);	
				//v = pObject->m_AnimTracks[30].fbxMatrix.MultT(v);
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];
				tVertex.c = TVector4(1,1,1,1);
				if (VertexColorSet)
				{
					FbxColor c = ReadColor(
						pFbxMesh,
						VertexColorSet,
						iCornerIndex[iIndex],
						iBasePolyIndex+ VertexColor[iIndex]);
					tVertex.c.x = c.mRed;
					tVertex.c.y = c.mGreen;
					tVertex.c.z = c.mBlue;
					tVertex.c.w = 1.0f;					
				}
				if (VertexUVSet)
				{
					FbxVector2 t = ReadTextureCoord(
						pFbxMesh,
						VertexUVSet,
						iCornerIndex[iIndex],
						iUVIndex[iIndex]);
					tVertex.t.x = t.mData[0];
					tVertex.t.y = 1.0f - t.mData[1];
				}
				if (VertexNormalSet)
				{
					FbxVector4 n = ReadNormal(
						pFbxMesh,
						VertexNormalSet,
						iCornerIndex[iIndex],
						iBasePolyIndex + VertexColor[iIndex]);
					n = normalLocalMatrix.MultT(n);
					tVertex.n.x = n.mData[0];
					tVertex.n.y = n.mData[2];
					tVertex.n.z = n.mData[1];
				}
				if (iNumMtrl <= 1)
				{
					pObject->m_VertexList.push_back(tVertex);
				}
				else
				{
					pObject->vbDataList[iSubMtrl].push_back(tVertex);
				}
			}
		}
		iBasePolyIndex += iPolySize;
	}

	m_pDrawObjList.push_back(pObject);
}
FbxVector4 TFbxLoader::ReadNormal(FbxMesh* pFbxMesh,
	FbxLayerElementNormal* VertexNormalSet,
	int posIndex,
	int colorIndex)
{
	FbxVector4 normal(1, 1, 1, 1);
	FbxLayerElement::EMappingMode mode = VertexNormalSet->GetMappingMode();
	switch (mode)
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			normal = VertexNormalSet->GetDirectArray().GetAt(posIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexNormalSet->GetIndexArray().GetAt(posIndex);
			normal = VertexNormalSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	} break;
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			normal = VertexNormalSet->GetDirectArray().GetAt(colorIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexNormalSet->GetIndexArray().GetAt(colorIndex);
			normal = VertexNormalSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	}break;
	}
	return normal;
}
FbxColor TFbxLoader::ReadColor(FbxMesh* pFbxMesh,
	FbxLayerElementVertexColor* VertexColorSet,
	int posIndex,
	int colorIndex)
{
	FbxColor color(1, 1, 1, 1);
	FbxLayerElement::EMappingMode mode = VertexColorSet->GetMappingMode();
	switch (mode)
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (VertexColorSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			color = VertexColorSet->GetDirectArray().GetAt(posIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexColorSet->GetIndexArray().GetAt(posIndex);
			color = VertexColorSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	} break;
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (VertexColorSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			color = VertexColorSet->GetDirectArray().GetAt(colorIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexColorSet->GetIndexArray().GetAt(colorIndex);
			color = VertexColorSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	}break;
	}
	return color;
}
FbxVector2 TFbxLoader::ReadTextureCoord(FbxMesh* pFbxMesh, 
	FbxLayerElementUV* pUVSet,
	int vertexIndex,
	int uvIndex)
{
	FbxVector2 t;
	/*enum EMappingMode
	{
		eNone,				// ������ �������� �ʾҴ�.
		eByControlPoint,	// ������ �� ������ 1���� ������ �Ǿ� �ִ�.
		eByPolygonVertex,	// �� ������ ���� �������� ���ο� ��ǥ�� �ִ�.
		eByPolygon,         // ��ü �����￡ ���� ��ǥ�� �ִ�.
		eByEdge,			// ������ 1���� ���� ��ǥ�� �ִ�.
		eAllSame			// ��ü �����￡ 1���� ���� ��ǥ�� �ִ�.
	};*/
	//enum EReferenceMode
	//{
	//	eDirect,   // n��° ���������� mDirectArray�� n��° ��ġ�� �ִ�.
	//	eIndex,    // fbx 5.0 ���� �������� ���Ǿ�����. �̻� ���������� eIndexToDirect�� ��ü�Ǿ���.
	//	eIndexToDirect
	//};
	// �ؽ�ó ���� ����� ����?
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
int TFbxLoader::GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* pMaterialSetList)
{
	// ���ι��
	//eNone,
	//eByControlPoint,  // ������
	//eByPolygonVertex, //  
	//eByPolygon, // �����︶�� �ٸ��� �ִ�.
	//eAllSame - ��üǥ�鿡 1���� ������ǥ�� �ִ�.
	int iSubMtrl = 0;
	if (pMaterialSetList != nullptr)
	{
		switch (pMaterialSetList->GetMappingMode())
		{
		case FbxLayerElement::eByPolygon:
		{
			// ���� ������ �迭�� ����Ǵ� ���
			switch (pMaterialSetList->GetReferenceMode())
			{
			case FbxLayerElement::eIndex:
			{
				iSubMtrl = iPoly;
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				iSubMtrl = pMaterialSetList->GetIndexArray().GetAt(iPoly);
			}break;
			}
		}
		default:
		{
			break;
		}
		}
	}
	return iSubMtrl;
}
void TFbxLoader::PreProcess(FbxNode* pFbxNode)
{	
	if (pFbxNode && (pFbxNode->GetCamera() || pFbxNode->GetLight()))	
	{
		return;
	}
	
	TFbxObject* pObject = new TFbxObject;
	std::string name = pFbxNode->GetName();
	pObject->m_szName = to_mw(name);
	pObject->m_pFbxNode = pFbxNode;
	pObject->m_pFbxParentNode = pFbxNode->GetParent();

	m_pObjectList.push_back(pObject);
	m_pObjectMap.insert(std::make_pair(pFbxNode, pObject));

	int iNumChild = pFbxNode->GetChildCount();
	for( int iChild=0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChild = pFbxNode->GetChild(iChild);
		// ���ۿ�����Ʈ + ������Ʈ�� ������Ʈ
		FbxNodeAttribute::EType type = pChild->GetNodeAttribute()->GetAttributeType();
		if( type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eSkeleton||
			type == FbxNodeAttribute::eNull)
		{
			PreProcess(pChild);
		}		
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