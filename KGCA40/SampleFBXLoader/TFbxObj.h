#pragma once
#include "TModel.h"
#include <fbxsdk.h>
#pragma comment	(lib, "libfbxsdk.lib")
#pragma comment	(lib, "libxml2-md.lib")
#pragma comment	(lib, "zlib-md.lib")
const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};
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
struct TWeight
{
	std::vector<int>     m_IndexList;
	std::vector<float>   m_WegihtList;
};
struct TSkinData
{
	std::vector<FbxNode*>  m_MatrixList;
	std::vector<TWeight>   m_VertexList;
};
struct PNCTIW_VERTEX : public PNCT_VERTEX
{
	int     index[4];
	float   weight[4];
};


struct TMesh : public TModel
{	
	// 정점 당 가중치, 인덱스 저장
	std::vector<PNCTIW_VERTEX>       m_WeightList;
	FbxNode* m_pFbxNode;
	OBJECTCLASSTYPE     m_ClassType;
	std::wstring		m_szName;
	std::wstring		m_szParentName;
	int					m_iNumLayer;
	std::vector<TLayer> m_LayerList;
	int					m_iMtrlRef;
	TMatrix				m_matWorld;	
	TMesh*				m_pParent;
	std::vector<TMatrix> m_AnimationTrack;
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
	TMesh()
	{
		m_ClassType = CLASS_GEOM;
	}
};
class TFbxObj 
{
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
public:
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime=0.0f;
	int		m_iAnimIndex=0;
	float m_fStartTime;
	float m_fEndTime;
	float m_fSampleTime;
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<TMtrl*>		m_pFbxMaterialList;
	std::vector<TMesh*>		m_pMeshList;
	CB_DATA					m_cbData;
/// <summary>
/// 가상함수 리스트
/// </summary>
public:
	bool		LoadObject(std::string filename);	
	bool		Frame();
	bool		Release();
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
public:
	void	SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
	bool    Render(ID3D11DeviceContext* pContext);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void	LoadMaterial(TMtrl* pMtrl);
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode, TMesh* pMesh);
	void	ParseMesh(FbxNode* pNode, TMesh* pMesh);	
	TMatrix   ParseTransform(FbxNode* pNode, TMatrix& matParent);
public:
	void	ParseAnimationNode(FbxNode* pNode, TMesh* pMesh);
	void	ParseAnimation();
	void	ParseAnimStack(FbxString* szData);
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, TMesh* pMesh, TSkinData* pSkindata);
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

