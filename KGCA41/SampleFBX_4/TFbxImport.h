#pragma once
#include <TObject.h>
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

struct TFbxMesh : TFbxModel
{
	UINT  m_iNumPolygon=0;
	using SUBMATERIAL = std::vector<PNCT_Vertex>;
	std::vector<SUBMATERIAL> 		m_TriangleList;
	std::vector<UINT> 				m_TriangleOffsetList;
	std::vector<W_STR>				m_szTextureFileName;	
	std::vector<TFbxMesh*>		    m_pChild;
};

class TFbxImport 
{
public:
	using FBX_MODEL = std::shared_ptr<TFbxModel>;
public:
	FbxManager*		m_pSDKManager	= nullptr;	 // 메모리 관리 
	FbxImporter*	m_pFbxImporter	= nullptr; // fbx파일을 로더
	FbxScene*		m_pFbxScene		= nullptr;		 // 화면 랜더링 오브젝트들.
	FbxNode*		m_FbxRootNode	= nullptr;
	std::vector<FbxNode*>    m_pNodeList;	
	std::vector<FBX_MODEL>   m_tModelList;
public:
	std::vector<FbxNode*>  m_pNodeMeshList;
	std::vector<TFbxMesh>  m_tMeshList;
public:
	TMatrix   ConvertMatrix(FbxMatrix& m);
	TMatrix   ConvertAMatrix(FbxAMatrix& m);
	TMatrix   DxConvertMatrix(TMatrix& m);
	TMatrix   ParseTransform(FbxNode* fbxNode);
public:
	bool      Load(W_STR filename);
	void	  PreProcess(FbxNode* fbxNode, TFbxModel* pParent=nullptr);
	void	  LoadMesh(FbxNode* node, TFbxMesh& tMesh);
	FbxVector2	ReadTextureCoord(FbxLayerElementUV* uvLayer, int iDxIndex, int uvIndex);
	FbxColor	ReadColor(FbxLayerElementVertexColor*, int iVertexIndex, int iIndex);
	FbxVector4  ReadNormal(FbxLayerElementNormal*, int iVertexIndex, int iIndex);
	C_STR		ParseMaterial(FbxSurfaceMaterial* pMtrl);
	int			GetSubMaterialIndex(UINT iPoly, FbxLayerElementMaterial* mtrl);
	bool      Init();
	bool      Release();
};

