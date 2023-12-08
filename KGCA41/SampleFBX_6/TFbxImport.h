#pragma once
#include "TFbxObject.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxImport 
{
public:
	UINT   m_iStartFrame = 0;
	UINT   m_iEndFrame = 0;
	UINT   m_iFrameSpeed = 30;
	UINT   m_iTickForFrame = 160;
public:
	FbxManager*		m_pSDKManager	= nullptr;	 // 메모리 관리 
	FbxImporter*	m_pFbxImporter	= nullptr; // fbx파일을 로더
	FbxScene*		m_pFbxScene		= nullptr;		 // 화면 랜더링 오브젝트들.
	FbxNode*		m_FbxRootNode	= nullptr;
	std::vector<FbxNode*>								m_pFbxNodeList;
	std::vector<std::shared_ptr<TFbxObj>>				m_pFbxNodeMeshList;
	std::map<FbxNode*, int>								m_pFbxNodeMap;
	//std::map<std::wstring, TFbxMesh*>	m_pFbxModelMap;
	DWORD		m_dwNodeIndex = 0;
public:
	TMatrix   ConvertMatrix(FbxMatrix& m);
	TMatrix   ConvertAMatrix(FbxAMatrix& m);
	TMatrix   DxConvertMatrix(TMatrix& m);
	TMatrix   ParseTransform(FbxNode* fbxNode);
public:
	void		GetAnimation(TFbxObj* fbxobj);
	bool		Load(W_STR filename, TFbxObj* obj);
	void		PreProcess(FbxNode* fbxNode, TFbxObj* fbxobj);
	void		LoadMesh(FbxNode* node, TFbxObj& tMesh);
	FbxVector2	ReadTextureCoord(FbxLayerElementUV* uvLayer, int iDxIndex, int uvIndex);
	FbxColor	ReadColor(FbxLayerElementVertexColor*, int iVertexIndex, int iIndex);
	FbxVector4  ReadNormal(FbxLayerElementNormal*, int iVertexIndex, int iIndex);
	C_STR		ParseMaterial(FbxSurfaceMaterial* pMtrl);
	int			GetSubMaterialIndex(UINT iPoly, FbxLayerElementMaterial* mtrl);
	bool      Init();
	bool      Release();


	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, TFbxObj* pObject);
};

