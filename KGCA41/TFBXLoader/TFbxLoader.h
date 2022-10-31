#pragma once
#include "TStd.h"
#include <fbxsdk.h>
#include "TObject3D.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxObject : public TBaseObject
{
public:
	std::vector<TFbxObject*> m_pDrawChild;
	std::vector< std::vector<SimpleVertex>>   vbDataList;
	std::vector< W_STR >   vbTexList;
public:
	void  CreateVertexData()
	{		
	}
	HRESULT CreateIndexBuffer()
	{
		return S_OK;
	}
};
class TFbxLoader
{
public:
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
	FbxNode*		m_pRootNode;
	std::vector< FbxMesh* > m_pFbxMeshList;
	std::vector< TFbxObject* > m_pDrawObjList;
	ID3D11DeviceContext* m_pContext=nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(C_STR filename);
	void PreProcess(FbxNode* pFbxNode);
	void ParseMesh(FbxMesh* pFbxMesh);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, 
								FbxLayerElementUV* VertexUVSet,
								int posIndex,
								int uvIndex);
	FbxColor ReadColor(FbxMesh* pFbxMesh,
		FbxLayerElementVertexColor* VertexColorSet,
		int posIndex,
		int colorIndex);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* MaterialSet);
};

