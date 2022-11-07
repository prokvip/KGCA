#pragma once
#include "TStd.h"
#include <fbxsdk.h>
#include "TFbxObject.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxLoader
{
public:
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
	FbxNode*		m_pRootNode;
	std::map<FbxNode*, TFbxObjectSkinning* > m_pObjectMap;
	std::map<FbxNode*, UINT > m_pObjectIDMap;
	std::vector< TFbxObjectSkinning* > m_pObjectList;

	std::vector< FbxMesh* > m_pFbxMeshList;
	std::vector< TFbxObjectSkinning* > m_pDrawObjList;
	ID3D11DeviceContext* m_pContext=nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(C_STR filename);
	void PreProcess(FbxNode* pFbxNode);
	void ParseMesh(FbxMesh* pFbxMesh, TFbxObjectSkinning* pObject);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, 
								FbxLayerElementUV* VertexUVSet,
								int posIndex,
								int uvIndex);
	FbxColor ReadColor(FbxMesh* pFbxMesh,
		FbxLayerElementVertexColor* VertexColorSet,
		int posIndex,
		int colorIndex);
	FbxVector4 ReadNormal(FbxMesh* pFbxMesh,
		FbxLayerElementNormal* VertexNormalSet,
		int posIndex,
		int colorIndex);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* MaterialSet);
	void LoadAnimation(TFbxObjectSkinning* pObj);
	TMatrix ConvertMatrix(FbxAMatrix& fbxMatrix);
	TMatrix DxConvertMatrix(FbxAMatrix& fbxMatrix);

};

