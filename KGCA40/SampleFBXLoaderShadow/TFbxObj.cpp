#define _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"
#include "TTimer.h"
#include <algorithm>
bool Compare(const pair<float, int>& a, const pair<float, int>& b)
{
	return a.first > b.first;
}
void TFbxObj::SetPixelShader(ID3D11PixelShader* ps)
{
	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		TMesh* pMesh = m_pMeshList[iMesh];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				TMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				if (ps == nullptr)
				{
					pSubMesh->m_pMainPS = pSubMesh->m_pPS;
				}
				else
				{
					pSubMesh->m_pMainPS = ps;
				}
			}
		}
		else
		{
			if (ps == nullptr)
			{
				pMesh->m_pMainPS = pMesh->m_pPS;
			}
			else
			{
				pMesh->m_pMainPS = ps;
			}
		}
	}
}
bool		TFbxObj::Frame()
{
	if (m_bAnimPlay)
	{
		m_fElpaseTime += g_fSecPerFrame * 1.0f;
		m_iAnimIndex = m_fElpaseTime * 30.0f;
		if (m_fEndTime < m_fElpaseTime)
		{
			m_iAnimIndex = 0;
			m_fElpaseTime = 0;
			m_bAnimPlay = false;
		}
	}
	/*for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		TMesh* pMesh = m_pMeshList[iObj];				
		m_matAnimMatrix.matAnimation[iObj] =
			pMesh->m_matBindPoseList[iObj] *
			pMesh->m_AnimationTrack[m_iAnimIndex];
	}*/
	return true;
}
void      TFbxObj::ParseNode(FbxNode* pNode, TMesh* pParentMesh)
{
	if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}
	TMesh* pMesh = new TMesh;
	pMesh->m_pFbxNode = pNode;
	pMesh->m_szName = TBASIS::mtw(pNode->GetName());	
	TMatrix matParent;
	if (pParentMesh != nullptr)
	{
		pMesh->m_szParentName = pParentMesh->m_szName;
		matParent = pParentMesh->m_matWorld;
	}
	pMesh->m_pParent = pParentMesh;
	pMesh->m_matWorld = ParseTransform(pNode, matParent);	

	if (pNode->GetMesh())
	{
		ParseMesh(pNode, pMesh);
		pMesh->m_ClassType = CLASS_GEOM;
	}
	else
	{
		pMesh->m_ClassType = CLASS_BONE;
	}
	m_pMeshList.push_back(pMesh);

	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		ParseNode(pChildNode, pMesh);
	}
}
TMatrix   TFbxObj::ParseTransform(FbxNode* pNode, TMatrix& matParent)
{
	//// TODO : 월드행렬
	FbxVector4 rotLcl = pNode->LclRotation.Get();
	FbxVector4 transLcl = pNode->LclTranslation.Get();
	FbxVector4 scaleLcl = pNode->LclScaling.Get();
	FbxMatrix matTransform(transLcl, rotLcl, scaleLcl);
	TMatrix matLocal = DxConvertMatrix(ConvertMatrix(matTransform));
	TMatrix matWorld = matLocal * matParent;
	return matWorld;
}
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
TMatrix     TFbxObj::ConvertAMatrix(FbxAMatrix& m)
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
void		TFbxObj::LoadMaterial(TMtrl* pMtrl)
{
	if (pMtrl == nullptr) return;
	if (pMtrl->m_pSubMtrl.size() > 0)
	{
		for (int iSub = 0; iSub < pMtrl->m_pSubMtrl.size(); iSub++)
		{
			TMtrl* pSubMtrl = pMtrl->m_pSubMtrl[iSub];
			FbxSurfaceMaterial* pFbxMaterial = pSubMtrl->m_pFbxSurfaceMtrl;
			FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (prop.IsValid())
			{
				int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
				for (int iTex = 0; iTex < iTexCount; iTex++)
				{
					FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
					if (pTex == nullptr) continue;
					FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

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
						if (_stricmp(Ext, ".tga") == 0)
						{
							szFileName += ".dds";
						}
						else
						{
							szFileName += Ext;
						}
					}

					pSubMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
					pSubMtrl->m_Texture.m_szFileName += L"object/";
					pSubMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
					pSubMtrl->m_Texture.LoadTexture(
						pSubMtrl->m_Texture.m_szFileName);
				}
			}
		}
	}
	else
	{
		//FbxSurfaceMaterial* pFbxSurfaceMtrl = pMtrl->m_pFbxSurfaceMtrl;
		FbxSurfaceMaterial* pFbxMaterial = pMtrl->m_pFbxNode->GetMaterial(0);
		FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (prop.IsValid())
		{
			int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
			for (int iTex = 0; iTex < iTexCount; iTex++)
			{
				FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
				if (pTex == nullptr) continue;
				FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

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
					if (_stricmp(Ext, ".tga") == 0)
					{
						szFileName += ".dds";
					}
					else
					{
						szFileName += Ext;
					}
				}

				pMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
				pMtrl->m_Texture.m_szFileName += L"object/";
				pMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
				pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
			}
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
		if (pMesh->m_ClassType != CLASS_GEOM) continue;		

		D3DXMatrixTranspose(&pMesh->m_matAnimMatrix.matAnimation[0],
							&pMesh->m_AnimationTrack[m_iAnimIndex]);

		for (int iBone = 0; iBone < pMesh->m_matBindPoseList.size(); iBone++)
		{
			TMatrix matAnim = pMesh->m_matBindPoseList[iBone] *
				pMesh->m_pMeshLinkList[iBone]->m_AnimationTrack[m_iAnimIndex];
			D3DXMatrixTranspose(&pMesh->m_matAnimMatrix.matAnimation[iBone],
				&matAnim);
		}
		TMtrl* pMtrl = nullptr;
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// 서브메터리얼 존재  
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
			{
				if (pMesh->m_pSubMesh[iSub]->m_pVertexList.size() <= 0) continue;
				TMtrl* pSubMtrl = 
					m_pFbxMaterialList[pMesh->m_iMtrlRef]->m_pSubMtrl[iSub];
				if (pSubMtrl->m_Texture.m_pTextureSRV != nullptr)
				{
					pContext->PSSetSamplers(0, 1, &pSubMtrl->m_Texture.m_pSampler);
					pContext->PSSetShaderResources(1, 1, &pSubMtrl->m_Texture.m_pTextureSRV);
				}
				// 스키닝도 서브메터리얼 사용 가능함.
				D3DXMatrixTranspose(&pMesh->m_pSubMesh[iSub]->m_matAnimMatrix.matAnimation[0],
					&pMesh->m_AnimationTrack[m_iAnimIndex]);
				for (int iBone = 0; iBone < pMesh->m_matBindPoseList.size(); iBone++)
				{
					pMesh->m_pSubMesh[iSub]->m_matAnimMatrix.matAnimation[iBone]
						= pMesh->m_matAnimMatrix.matAnimation[iBone];
				}
				pMesh->m_pSubMesh[iSub]->SetMatrix(&m_matWorld,
					&m_cbData.matView, &m_cbData.matProj);
				pMesh->m_pSubMesh[iSub]->Render(pContext);				
			}
		}
		else 
		{
			if (pMesh->m_iMtrlRef >= 0)
			{
				pMtrl = m_pFbxMaterialList[pMesh->m_iMtrlRef];
			}			
			if (pMtrl != nullptr)
			{
				if (pMtrl->m_Texture.m_pTextureSRV != nullptr)
				{
					pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
					pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pTextureSRV);
				}
			}
			pMesh->SetMatrix(&m_cbData.matWorld,&m_cbData.matView, &m_cbData.matProj);
			pMesh->Render(pContext);
		}
	}
	return true;
}
int     TFbxObj::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		if (m_pFbxMaterialList[iMtrl]->m_pFbxSurfaceMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
}
void	TFbxObj::ParseMesh(FbxNode* pNode, TMesh* pMesh)
{
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->m_iMtrlRef = GetRootMtrl(pFbxMaterial);

	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;
	if (pFbxMesh != nullptr)
	{
		int iNumCP = pFbxMesh->GetControlPointsCount();
		// 스키닝 오브젝트 여부?
		// pFbxMesh에 영향을 미치는 행렬(노드)에 전체 개수?
		// 행렬[0]=FbxNode ~ 행렬[3] = 4개
		// 정점[0]->인덱스[1]		// 		
		TSkinData skindata;
		bool bSkinnedMesh = ParseMeshSkinning(pFbxMesh, pMesh, &skindata);		
		if (bSkinnedMesh)
		{			
			_ASSERT(skindata.m_VertexList.size() == iNumCP);
			for (int i = 0; i < skindata.m_VertexList.size(); i++)
			{
				std::vector<std::pair<float, int>> list;
				for (int j = 0; j <
					skindata.m_VertexList[i].m_IndexList.size();
					j++)
				{
					list.push_back(std::make_pair(
						skindata.m_VertexList[i].m_WegihtList[j],
						skindata.m_VertexList[i].m_IndexList[j]));
				}
				std::sort(list.begin(), list.end(), Compare);
				for (int k = 0; k < list.size(); k++)
				{
					skindata.m_VertexList[i].m_WegihtList[k]=list[k].first;
					skindata.m_VertexList[i].m_IndexList[k]=list[k].second;					
				}
			}			
		}

		pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
		pMesh->m_LayerList.resize(pMesh->m_iNumLayer);
	
		// todo : 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < pMesh->m_iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
			if (pLayer->GetVertexColors() != nullptr) 
			{
				pMesh->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
			}
			if (pLayer->GetNormals() != nullptr) 
			{
				pMesh->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
			}
			if (pLayer->GetUVs() != nullptr) 
			{
				pMesh->m_LayerList[iLayer].pUV = pLayer->GetUVs();
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
			}
		}
		// TODO : 기하행렬
		FbxAMatrix matGeom;			
		FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		matGeom.SetT(trans);
		matGeom.SetR(rot);
		matGeom.SetS(scale);

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		
		int iBasePolyIndex = 0;
		int iNumFbxMaterial = pNode->GetMaterialCount();
		if (iNumFbxMaterial > 1)
		{
			pMesh->m_pSubMesh.resize(iNumFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				pMesh->m_pSubMesh[iSub] = new TMesh;
			}
		}

		FbxLayerElementMaterial* fbxSubMaterial = 
			pMesh->m_LayerList[0].pMaterial;
		FbxLayerElementUV* VertexUVList=
			pMesh->m_LayerList[0].pUV;
		FbxLayerElementVertexColor* VertexColorList=
			pMesh->m_LayerList[0].pColor;
		FbxLayerElementNormal* VertexNormalList=
			pMesh->m_LayerList[0].pNormal;

		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			int iSubMtrlIndex = 0;
			if (fbxSubMaterial != nullptr)
			{
				switch (fbxSubMaterial->GetMappingMode())
				{
					case FbxLayerElement::eByPolygon: 
					{
						switch (fbxSubMaterial->GetReferenceMode())
						{
							case FbxLayerElement::eDirect:
							{
								iSubMtrlIndex = iPoly;
							}break;
							case FbxLayerElement::eIndex:
							case FbxLayerElement::eIndexToDirect:
							{
								iSubMtrlIndex = 
									fbxSubMaterial->GetIndexArray().GetAt(iPoly);
							}break;
						}
					}break;
					default:
					{
						iSubMtrlIndex = 0;
					}break;
				}
			}
			if (iSubMtrlIndex < 0 || iSubMtrlIndex >= iNumFbxMaterial)
			{
				int kk = 0;
			}
			// 삼각형, 사각형
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iVertexIndex[3];
			for (int iTriangle = 0; iTriangle < m_iNumTriangle;	iTriangle++)
			{
				// 위치 인덱스
				iVertexIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iVertexIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iVertexIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);
				
				for (int iIndex = 0;iIndex < 3;	iIndex++)
				{
					PNCT_VERTEX vertex;
					PNCTIW_VERTEX iwVertex;
					ZeroMemory(&iwVertex, sizeof(PNCTIW_VERTEX));
					FbxVector4 pos = pVertexPositions[iVertexIndex[iIndex]];
					FbxVector4 vPos = matGeom.MultT(pos);
					vertex.pos.x = vPos.mData[0];
					vertex.pos.y = vPos.mData[2];
					vertex.pos.z = vPos.mData[1];
					if (VertexUVList != nullptr)
					{
						// UV 인덱스
						int uvIndex[3];
						uvIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
						uvIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
						uvIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, VertexUVList,
							iVertexIndex[iIndex], uvIndex[iIndex]	);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f-uv.mData[1];
					}					
					if (VertexColorList != nullptr)
					{
						int iColorIndex[3];
						iColorIndex[0] = iBasePolyIndex + 0;
						iColorIndex[1] = iBasePolyIndex + iTriangle + 2;
						iColorIndex[2] = iBasePolyIndex + iTriangle + 1;

						FbxColor color = ReadColor(
							pFbxMesh, 1, VertexColorList,
							iVertexIndex[iIndex], iColorIndex[iIndex]);
						vertex.color.x = color.mRed;
						vertex.color.y = color.mGreen;
						vertex.color.z = color.mBlue;
						vertex.color.w = 1.0f;
						
					}
					if (VertexNormalList != nullptr)
					{
						int iNormalIndex[3];
						iNormalIndex[0] = iBasePolyIndex + 0;
						iNormalIndex[1] = iBasePolyIndex + iTriangle + 2;
						iNormalIndex[2] = iBasePolyIndex + iTriangle + 1;
						FbxVector4 normal = ReadNormal(
							pFbxMesh, 1, VertexNormalList,
							iVertexIndex[iIndex], iNormalIndex[iIndex]);
						vertex.normal.x = normal.mData[0];
						vertex.normal.y = normal.mData[2];
						vertex.normal.z = normal.mData[1];
					}
					// 인덱스 및 가중치 저장
					int iRealIndex = iVertexIndex[iIndex];
					if (bSkinnedMesh)
					{						
						int iNum = 
							skindata.m_VertexList[iRealIndex].m_IndexList.size();
						for (int i=0; i < min(iNum,4); i++)
						{
							iwVertex.index[i] =
								skindata.m_VertexList[iRealIndex].m_IndexList[i];
							iwVertex.weight[i] =
								skindata.m_VertexList[iRealIndex].m_WegihtList[i];
						}
					}
					// 비 스키닝 오브젝트를 -> 스키닝화 처리
					else
					{
						iwVertex.index[0] = 0;
						iwVertex.weight[0] = 1.0f;
						for (int i = 1; i <4; i++)
						{
							iwVertex.index[i] = 0;
							iwVertex.weight[i] =0.0f;
						}
					}
					if (iNumFbxMaterial > 1)
					{			
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_pVertexList.push_back(vertex);
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_WeightList.push_back(iwVertex);
					}
					else
					{
						pMesh->m_pVertexList.push_back(vertex);
						pMesh->m_WeightList.push_back(iwVertex);
					}
				}
			}
			iBasePolyIndex += iPolySize;
		}
	}
}
void	TFbxObj::PreProcess(FbxNode* pNode)
{
	if (pNode && (pNode->GetCamera() || pNode->GetLight())) { return; }
	m_pFbxNodeList.push_back(pNode);	
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (GetRootMtrl(pFbxMaterial) == -1)
	{
		if (iNumFbxMaterial > 1 )
		{
			TMtrl* pMtrl = new TMtrl(pNode, pFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
				_ASSERT(pFbxSubMaterial != nullptr);
				TMtrl* pSubMtrl = new TMtrl(pNode, pFbxSubMaterial);
				pMtrl->m_pSubMtrl.push_back(pSubMtrl);
			}
			m_pFbxMaterialList.push_back(pMtrl);
		}
		else
		{
			if (pFbxMaterial != nullptr)
			{
				TMtrl* pMtrl = new TMtrl(pNode, pFbxMaterial);
				m_pFbxMaterialList.push_back(pMtrl);
			}
		}
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);	
		PreProcess(pChildNode);
	}
}
bool	TFbxObj::LoadObject(std::string filename, std::string shaderName)
{
	m_pFbxManager = FbxManager::Create();	
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager,"");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	bool bRet = m_pFbxImporter->Initialize( filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);

	FbxNode* m_pRootNode=m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode);
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		TMtrl* pMtrl = m_pFbxMaterialList[iMtrl];		
		LoadMaterial(pMtrl);
	}

	ParseAnimation();
	ParseNode(m_pRootNode, nullptr);
	ParseAnimationNode();

	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		TMesh* pMesh = m_pMeshList[iMesh];
		for (int iBone = 0; iBone < pMesh->m_pFbxNodeList.size(); iBone++)
		{
			TMesh* pLinkMesh = GetFindInedx(pMesh->m_pFbxNodeList[iBone]);
			pMesh->m_pMeshLinkList.push_back(pLinkMesh);
		}
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				TMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				// todo : 쉐이더 등등 중복처리 미작업
				pSubMesh->Create(TBASIS::mtw(shaderName), 
								 TBASIS::mtw(shaderName),
								 L"");
			}
		}
		else
		{
			pMesh->Create(	TBASIS::mtw(shaderName), 
							TBASIS::mtw(shaderName),L"");
		}
	}
	m_pFbxScene->Destroy();	
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();
	return bRet;
}
bool    TFbxObj::Release()
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
		SAFE_DEL(m_pMeshList[iObj]);
	}
	for (int iObj = 0; iObj < m_pFbxMaterialList.size(); iObj++)
	{
		m_pFbxMaterialList[iObj]->Release();
		SAFE_DEL(m_pFbxMaterialList[iObj]);
	}
	return true;
}
