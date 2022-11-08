#pragma once
#include "TStd.h"
#include <fbxsdk.h>
#include "TFbxObject.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxFile
{
public:
	TAnimScene  m_AnimScene;
	float       m_fAnimFrame = 0;
	float       m_fAnimInverse = 1.0f;
	float       m_fAnimSpeed = 1.0f;
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

	VS_CONSTANT_BONE_BUFFER  m_cbDataBone;
	ID3D11Buffer* m_pConstantBufferBone;
	HRESULT	CreateConstantBuffer(ID3D11Device* pDevice);
	
public:
	bool Init();
	bool UpdateFrame(ID3D11DeviceContext* pContext);
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
	void InitAnimation();
	void LoadAnimation(FbxLongLong t,FbxTime time);
	TMatrix ConvertMatrix(FbxAMatrix& fbxMatrix);
	TMatrix DxConvertMatrix(FbxAMatrix& fbxMatrix);
	bool ParseMeshSkinning(FbxMesh* pFbxMesh, TFbxObjectSkinning* pObject);
};

