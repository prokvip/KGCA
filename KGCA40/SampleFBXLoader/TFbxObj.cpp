#define _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"
TMatrix     TFbxObj::DxConvertMatrix(TMatrix m)
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
TMatrix     TFbxObj::ConvertMatrix(FbxMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast< double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
void		TFbxObj::LoadMaterial(TMtrl* pMtrl)
{
	//FbxSurfaceMaterial* pFbxSurfaceMtrl = pMtrl->pFbxMtrl;
	FbxSurfaceMaterial* pFbxMaterial = pMtrl->m_pFbxNode->GetMaterial(0);
	FbxProperty prop= pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (prop.IsValid())
	{
		int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
		for (int iTex = 0; iTex < iTexCount; iTex++)
		{
			FbxTexture* pTex =	prop.GetSrcObject<FbxFileTexture>(iTex);
			if (pTex == nullptr) continue;
			FbxFileTexture* fileTexture =prop.GetSrcObject<FbxFileTexture>(iTex);
			
			std::string szFileName;
			char Drive[MAX_PATH] = { 0, };
			char Dir[MAX_PATH] = { 0, };
			char FName[MAX_PATH] = { 0, };
			char Ext[MAX_PATH] = { 0, };
			if (fileTexture->GetFileName())
			{
				_splitpath(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
				Ext[4] = 0;
				szFileName = FName;
				szFileName += Ext;
			}
			pMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
			pMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
			pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
		}						
	}
}
void		TFbxObj::SetMatrix(
	TMatrix* pMatWorld,
	TMatrix* pMatView, TMatrix* pMatProj)
{
	if (pMatWorld != nullptr)
	{
		m_cbData.matWorld = *pMatWorld;
	}
	if (pMatView != nullptr)
	{
		m_cbData.matView = *pMatView;
	}
	if (pMatProj != nullptr)
	{
		m_cbData.matProj = *pMatProj;
	}
}
bool    TFbxObj::Render(ID3D11DeviceContext* pContext)
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		TMesh* pMesh = m_pMeshList[iObj];
		TMtrl* pMtrl = nullptr;
		if (pMesh->m_iMtrlRef >= 0)
		{
			pMtrl = m_pFbxMaterialList[pMesh->m_iMtrlRef];
		}
		pMesh->SetMatrix(&pMesh->m_matWorld, &m_cbData.matView, &m_cbData.matProj);
		if (pMtrl != nullptr)
		{
			pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
			pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pTextureSRV);
		}
		pMesh->Render(pContext);
	}
	return true;
}
int     TFbxObj::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		if (m_pFbxMaterialList[iMtrl]->pFbxMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
}
void	TFbxObj::ParseNode(FbxNode* pNode, TMesh* pMesh)
{
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->m_iMtrlRef = GetRootMtrl(pFbxMaterial);
	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;
	if (pFbxMesh != nullptr)
	{
		int iNumLayer = pFbxMesh->GetLayerCount();
		std::vector< FbxLayerElementUV*> VertexUVList;
		std::vector< FbxLayerElementVertexColor*> VertexColorList;
		std::vector< FbxLayerElementNormal*> VertexNormalList;
		// todo : 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(0);
			if (pLayer->GetVertexColors() != nullptr) 
			{
				VertexColorList.push_back(pLayer->GetVertexColors());
			}
			if (pLayer->GetNormals() != nullptr) 
			{
				VertexNormalList.push_back(pLayer->GetNormals());
			}
			if (pLayer->GetUVs() != nullptr) 
			{
				VertexUVList.push_back(pLayer->GetUVs());
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				FbxLayerElementMaterial* fbxMaterial = pLayer->GetMaterials();
				//	int iMtrlCount = fbxMaterial->mDirectArray->GetCount();
				//	// 서브메터리얼 구조
				//	if (iMtrlCount > 1)
				//	{
				//		for (int iSub=0;iSub < iMtrlCount; iSub++)
				//		{

				//		}
				//	}
				//	else
				//	{
				//		FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
				//		FbxProperty prop= pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
				//		if (prop.IsValid())
				//		{
				//			int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
				//			for (int iTex = 0; iTex < iTexCount; iTex++)
				//			{
				//				FbxTexture* pTex =	prop.GetSrcObject<FbxFileTexture>(iTex);
				//				if (pTex == nullptr) continue;
				//				FbxFileTexture* fileTexture =prop.GetSrcObject<FbxFileTexture>(iTex);
				//				fbxFileTexList.push_back(fileTexture->GetFileName());
				//			}						
				//		}
				//	}
				//}
			}
		}
		// TODO : 월드행렬
		FbxAMatrix matGeom;
		{
			FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
			matGeom.SetT(trans);
			matGeom.SetR(rot);
			matGeom.SetS(scale);
		}
		FbxMatrix matA = matGeom;
		pMesh->m_matWorld = DxConvertMatrix(ConvertMatrix(matA));
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
					//FbxVector4 vPos = matGeom.MultT(pos);
					vertex.pos.x = pos.mData[0];
					vertex.pos.y = pos.mData[2];
					vertex.pos.z = pos.mData[1];
					if (VertexUVList.size())
					{
						// todo : uvlist
						FbxLayerElementUV* pUVElement=
							VertexUVList[0];
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, pUVElement,
							iCornerIndex[iIndex], u[iIndex]	);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f-uv.mData[1];
					}					
					if (VertexColorList.size())
					{
						// todo : uvlist
						FbxLayerElementVertexColor* pElement = VertexColorList[0];
						FbxColor color = ReadColor(
							pFbxMesh, 1, pElement,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.color.x = color.mRed;
						vertex.color.y = color.mGreen;
						vertex.color.z = color.mBlue;
						vertex.color.w = 1.0f;
						
					}
					if (VertexNormalList.size())
					{
						// todo : uvlist
						FbxLayerElementNormal* pElement =
							VertexNormalList[0];
						FbxVector4 normal = ReadNormal(
							pFbxMesh, 1, pElement,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.normal.x = normal.mData[0];
						vertex.normal.y = normal.mData[2];
						vertex.normal.z = normal.mData[1];
					}
					pMesh->m_pVertexList.push_back(vertex);
				}
			}
		}
	}
}

void	TFbxObj::PreProcess(FbxNode* pNode)
{
	// pNode 정보 얻기
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (pFbxMaterial != nullptr)
	{
		TMtrl* pMtrl = new TMtrl;
		pMtrl->pFbxMtrl = pFbxMaterial;
		pMtrl->m_pFbxNode = pNode;			
		m_pFbxMaterialList.push_back(pMtrl);
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);	
		FbxNodeAttribute::EType type = 
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh )
		{
			m_pFbxNodeList.push_back(pChildNode);
		}
		PreProcess(pChildNode);
	}
}
bool	TFbxObj::LoadObject(std::string filename)
{
	// todo  : 메모리 관리;
	m_pFbxManager = FbxManager::Create();	
	//todo  : FbxImporter* m_pFbxImporter;
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager,"");
	//todo  : FbxScene* m_pFbxScene;
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	bool bRet = m_pFbxImporter->Initialize( filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	FbxNode* m_pRootNode=m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode);

	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		TMtrl* pMtrl = m_pFbxMaterialList[iMtrl];		
		LoadMaterial(pMtrl);
	}

	for (int iObj = 0; iObj < m_pFbxNodeList.size(); iObj++)
	{
		FbxNode* pNode = m_pFbxNodeList[iObj];
		TMesh* pMesh = new TMesh;
		m_pMeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);
		pMesh->Create(L"FbxShader.hlsl", L"../../data/shader/DefaultShader.hlsl");
	}	
	return bRet;
}
bool    TFbxObj::Release()
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
	}
	for (int iObj = 0; iObj < m_pFbxMaterialList.size(); iObj++)
	{
		m_pFbxMaterialList[iObj]->Release();
		delete m_pFbxMaterialList[iObj];
	}
	return true;
}
