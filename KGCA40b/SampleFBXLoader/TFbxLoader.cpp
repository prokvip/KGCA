#define  _CRT_SECURE_NO_WARNINGS
#include "TFbxLoader.h"

TMatrix     TFbxLoader::DxConvertMatrix(TMatrix m)
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
TMatrix     TFbxLoader::ConvertMatrix(FbxMatrix& m)
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
TMatrix     TFbxLoader::ConvertAMatrix(FbxAMatrix& m)
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
void		TFbxLoader::ParseAnimation()
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	FbxString TakeName = stack->GetName();
	FbxTakeInfo* TakeInfo = m_pFbxScene->GetTakeInfo(TakeName);
	FbxTimeSpan LocalTimeSpan = TakeInfo->mLocalTimeSpan;
	FbxTime start		= LocalTimeSpan.GetStart();
	FbxTime end			= LocalTimeSpan.GetStop();
	FbxTime Duration	= LocalTimeSpan.GetDuration();

	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong s = start.GetFrameCount(TimeMode);
	FbxLongLong n = end.GetFrameCount(TimeMode);

	// 1초에 30 frame 
	// 1Frame = 160 Tick
	// 50 Frame 
	FbxTime time;
	TTrack tTrack;
	for (FbxLongLong t = s; t <= n; t++)
	{
		time.SetFrame(t, TimeMode);
		for (int iObj = 0; iObj < m_TreeList.size(); iObj++)
		{
			FbxAMatrix matGlobal =	m_TreeList[iObj]->m_pFbxNode->EvaluateGlobalTransform(time);			
			tTrack.iFrame = t;
			tTrack.matTrack = DxConvertMatrix(ConvertAMatrix(matGlobal));
			m_TreeList[iObj]->m_AnimTrack.push_back(tTrack);		
		}
	}
}
void    TFbxLoader::PreProcess(FbxNode* node, TFbxObj* fbxParent)
{
	TFbxObj* fbx = nullptr;
	if (node != nullptr)
	{
		fbx = new TFbxObj;
		fbx->m_pFbxParent = node->GetParent();
		fbx->m_pFbxNode = node;
		fbx->m_csName = to_mw(node->GetName());
		fbx->m_pParentObj = fbxParent;
		m_TreeList.push_back(fbx);
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
bool	TFbxLoader::Load(std::string filename)
{
	bool bRet = m_pFbxImporter->Initialize(filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	m_pRootNode = m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode, nullptr);
	ParseAnimation();
	for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
	{
		ParseMesh(m_DrawList[iObj]);		
	}
	return true;
}
void	TFbxLoader::ParseMesh(TFbxObj* pObject)
{
	FbxMesh* pFbxMesh = pObject->m_pFbxNode->GetMesh();
	// 기하행렬(초기 정점 위치를 변환할 때 사용)
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

	// 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)
	std::vector<FbxLayerElementUV*> VertexUVSet;
	std::vector<FbxLayerElementVertexColor*> VertexColorSet;
	std::vector<FbxLayerElementMaterial*> MaterialSet;
	int iLayerCount = pFbxMesh->GetLayerCount();
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
		if (pFbxLayer->GetMaterials() != nullptr)
		{
			MaterialSet.push_back(pFbxLayer->GetMaterials());
		}
	}

	//  1개의 오브젝트가 여러장의 텍스처를 사용한다.
	//  각각의 텍스처를 이름을 얻고 저장한다.
	//  어떤 페이스(폴리곤)가 어떤 텍스처를 사용하니?
	//  같은 텍스처를 사용하는 폴리곤들 끼리 저장한다.
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
				I_Texture.Load(pObject->m_szTexFileList[iMtrl]));
		}
	}
	if(pObject->m_szTexFileList.size() > 0)
	{
		pObject->m_szTexFileName = pObject->m_szTexFileList[0];
	}
	if (iNumMtrl > 0)
	{
		pObject->m_pSubVertexList.resize(iNumMtrl);
	}
	else
	{
		pObject->m_pSubVertexList.resize(1);
	}

	int iBasePolyIndex = 0;
	// 삼각형, 사각형
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
			CornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[0]);
			CornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[1]);
			CornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[2]);
			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				TVertex tVertex;	
				// Max(x,z,y) ->(dx)x,y,z    
				FbxVector4 v = pVertexPositions[CornerIndex[iIndex]];
				v = geom.MultT(v);
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];

				// uv
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[2]);
				if (VertexUVSet.size() > 0)
				{
					FbxLayerElementUV* pUVSet = VertexUVSet[0];
					FbxVector2 uv;
					ReadTextureCoord(
						pFbxMesh,
						pUVSet,
						CornerIndex[iIndex],
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
						CornerIndex[iIndex],
						iBasePolyIndex + VertexIndex[iIndex]);
				}
				tVertex.c.x = color.mRed;
				tVertex.c.y = color.mGreen;
				tVertex.c.z = color.mBlue;
				tVertex.c.w = 1;


				FbxVector4 normal = ReadNormal(pFbxMesh,
					CornerIndex[iIndex],
					iBasePolyIndex + VertexIndex[iIndex]);
				normal = normalMatrix.MultT(normal);
				tVertex.n.x = normal.mData[0]; // x
				tVertex.n.y = normal.mData[2]; // z
				tVertex.n.z = normal.mData[1]; // y

				//pObject->m_VertexList.push_back(tVertex);//36
				pObject->m_pSubVertexList[iSubMtrl].push_back(tVertex);
			}
		}

		iBasePolyIndex += iPolySize;
	}
	
}
bool	TFbxLoader::Init()
{
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");	
	return true;
}
bool	TFbxLoader::Frame()
{
	return true;
}
bool	TFbxLoader::Render()
{
	return true;
}
bool	TFbxLoader::Release()
{
	for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
	{
		m_DrawList[iObj]->Release();
	}
	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();	
	m_pFbxManager->Destroy();
	return true;
}