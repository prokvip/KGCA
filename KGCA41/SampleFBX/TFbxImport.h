#pragma once
#include <TObject.h>
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxImport
{
public:
	FbxManager*		m_pSDKManager	= nullptr;	 // 메모리 관리 
	FbxImporter*	m_pFbxImporter	= nullptr; // fbx파일을 로더
	FbxScene*		m_pFbxScene		= nullptr;		 // 화면 랜더링 오브젝트들.
	FbxNode*		m_FbxRootNode	= nullptr;
	std::vector<FbxNode*>  m_pNodeList;
public:
	bool      Load(W_STR filename);
	void	  PreProcess(FbxNode* fbxNode);
	void	  LoadMesh(FbxNode* node);
	bool      Init();
	bool      Release();
};

