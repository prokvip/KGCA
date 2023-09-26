#pragma once
#include "TObject.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

struct TFbxModel
{
	C_STR      m_csName;
	TMatrix    m_matWorld;
	TFbxModel* m_pParent = nullptr;
};

struct TFbxMesh
{
	C_STR      m_csName;
	TMatrix    m_matWorld;
	TFbxMesh* m_pParent = nullptr;

	UINT  m_iNumPolygon = 0;
	using SUBMATERIAL = std::vector<PNCT_Vertex>;
	std::vector<SUBMATERIAL> 		m_TriangleList;
	std::vector<UINT> 				m_TriangleOffsetList;
	std::vector<W_STR>				m_szTextureFileName;
	std::vector<TMatrix>		    m_MatrixArray;
	std::vector<TFbxMesh*>		    m_pChild;
};


struct TFbxObj
{
	W_STR     m_csName;
	UINT	m_iStartFrame = 0;
	UINT	m_iEndFrame = 0;
	UINT	m_iFrameSpeed = 30;
	UINT	m_iTickForFrame = 160;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	std::vector< PNCT_Vertex> m_VertexList;
	std::vector< DWORD>       m_IndexList;
	std::vector<std::shared_ptr<TFbxMesh>>		    m_tMeshList;
};
using FBX_MODEL = std::shared_ptr<TFbxModel>;
class TFbxImport 
{
public:
	UINT   m_iStartFrame = 0;
	UINT   m_iEndFrame = 0;
	UINT   m_iFrameSpeed = 30;
	UINT   m_iTickForFrame = 160;
public:
public:
	FbxManager*		m_pSDKManager	= nullptr;	 // 메모리 관리 
	FbxImporter*	m_pFbxImporter	= nullptr; // fbx파일을 로더
	FbxScene*		m_pFbxScene		= nullptr;		 // 화면 랜더링 오브젝트들.
	FbxNode*		m_FbxRootNode	= nullptr;
	std::vector<FbxNode*>		m_pNodeList;	
	std::vector<FbxNode*>		m_pFbxNodeMeshList;
	std::vector<FBX_MODEL>		m_tModelList;
	//std::vector<TFbxMesh>		m_tMeshList;
public:
	TMatrix   ConvertMatrix(FbxMatrix& m);
	TMatrix   ConvertAMatrix(FbxAMatrix& m);
	TMatrix   DxConvertMatrix(TMatrix& m);
	TMatrix   ParseTransform(FbxNode* fbxNode);
public:
	void		GetAnimation(TFbxObj* fbxobj);
	bool		Load(W_STR filename, TFbxObj* obj);
	void		PreProcess(FbxNode* fbxNode, TFbxModel* pParent=nullptr);
	void		LoadMesh(FbxNode* node, TFbxMesh& tMesh);
	FbxVector2	ReadTextureCoord(FbxLayerElementUV* uvLayer, int iDxIndex, int uvIndex);
	FbxColor	ReadColor(FbxLayerElementVertexColor*, int iVertexIndex, int iIndex);
	FbxVector4  ReadNormal(FbxLayerElementNormal*, int iVertexIndex, int iIndex);
	C_STR		ParseMaterial(FbxSurfaceMaterial* pMtrl);
	int			GetSubMaterialIndex(UINT iPoly, FbxLayerElementMaterial* mtrl);
	bool      Init();
	bool      Release();
};

