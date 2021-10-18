#pragma once
#include "TModel.h"
#include <fbxsdk.h>
#pragma comment	(lib, "libfbxsdk.lib")
#pragma comment	(lib, "libxml2-md.lib")
#pragma comment	(lib, "zlib-md.lib")
struct TMtrl
{	
	FbxNode*			m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	TTexture			m_Texture;
	std::vector<TMtrl*> m_pSubMtrl;
	TMtrl() {}
	TMtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}
	void Release()
	{
		m_Texture.Release();
		for (auto data : m_pSubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
struct TLayer
{
	FbxLayerElementUV*			pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal*		pNormal;
	FbxLayerElementMaterial*	pMaterial;
};
struct TMesh : public TModel
{
	int					m_iNumLayer;
	std::vector<TLayer> m_LayerList;
	int					m_iMtrlRef;
	TMatrix				m_matWorld;	
	std::vector<TMesh*> m_pSubMesh;	
	bool Release() override
	{
		TModel::Release();
		for (auto data : m_pSubMesh)
		{
			data->Release();
			SAFE_DEL(data);
		}
		return true;
	}
};
class TFbxObj 
{
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
public:
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<TMtrl*>		m_pFbxMaterialList;
	std::vector<TMesh*>		m_pMeshList;
	CB_DATA					m_cbData;
/// <summary>
/// 가상함수 리스트
/// </summary>
public:
	bool		LoadObject(std::string filename);	
	bool		Release();
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
public:
	void	SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
	bool    Render(ID3D11DeviceContext* pContext);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void	LoadMaterial(TMtrl* pMtrl);
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode, TMesh* pMesh);
public:
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,
		DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
};

