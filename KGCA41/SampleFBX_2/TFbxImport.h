#pragma once
#include <TObject.h>
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

struct TFbxMesh
{
	std::vector<PNCT_Vertex> 		m_TriangleList;
};

class TFbxImport 
{
public:
	FbxManager*		m_pSDKManager	= nullptr;	 // �޸� ���� 
	FbxImporter*	m_pFbxImporter	= nullptr; // fbx������ �δ�
	FbxScene*		m_pFbxScene		= nullptr;		 // ȭ�� ������ ������Ʈ��.
	FbxNode*		m_FbxRootNode	= nullptr;
	std::vector<FbxNode*>  m_pNodeMeshList;
public:
	std::vector<TFbxMesh>  m_tMeshList;	
public:
	bool      Load(W_STR filename);
	void	  PreProcess(FbxNode* fbxNode);
	void	  LoadMesh(FbxNode* node, TFbxMesh& tMesh);
	FbxVector2 ReadTextureCoord(FbxLayerElementUV* uvLayer, int uvIndex);
	bool      Init();
	bool      Release();
};

