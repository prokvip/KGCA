#pragma once
#include "TModel.h"
#include <fbxsdk.h>
#ifdef _DEBUG
#pragma comment	(lib, "libfbxsdk-md.lib")
#pragma comment	(lib, "libxml2-md.lib")
#pragma comment	(lib, "zlib-md.lib")
#else
//#pragma comment	(lib, "libfbxsdk.lib")
#pragma comment	(lib, "libfbxsdk-md.lib")
#pragma comment	(lib, "libxml2-md.lib")
#pragma comment	(lib, "zlib-md.lib")
#endif

const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};
struct TMtrl
{
	FbxNode* m_pFbxNode;
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
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
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
struct PNCTIW_VERTEX 
{
	float   index[4];
	float   weight[4];
};
struct TAnimMatrix
{
	TMatrix   matAnimation[255];
};
class TMesh :  public TModel
{
public:	
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<TMesh*>     m_pMeshLinkList;
	std::vector<TMatrix>	m_matBindPoseList;
	
	TAnimMatrix   m_matAnimMatrix;
	ID3D11Buffer* m_pAnimCB = nullptr;
	ID3D11Buffer* m_pIWVertrexBuffer = nullptr;
	// 정점 당 가중치, 인덱스 저장
	std::vector<PNCTIW_VERTEX>       m_WeightList;
	FbxNode* m_pFbxNode = nullptr;
	OBJECTCLASSTYPE     m_ClassType;
	std::wstring		m_szName;
	std::wstring		m_szParentName;
	int					m_iNumLayer = 0;
	std::vector<TLayer> m_LayerList;
	int					m_iMtrlRef;
	TMatrix				m_matWorld;
	TMesh* m_pParent = nullptr;
	std::vector<TMatrix> m_AnimationTrack;
	std::vector<TMesh*> m_pSubMesh;	
public:
	bool Release() override;
	virtual HRESULT		CreateConstantBuffer()override;
	virtual HRESULT		CreateVertexLayout() override;
	virtual HRESULT		CreateVertexBuffer()override;
	virtual bool		PreRender(ID3D11DeviceContext* pContext)override;
public:
	TMesh();
	virtual ~TMesh();
};

