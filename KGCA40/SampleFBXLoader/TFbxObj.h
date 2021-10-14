#pragma once
#include "TModel.h"
#include <fbxsdk.h>
#pragma comment	(lib, "libfbxsdk.lib")
#pragma comment	(lib, "libxml2-md.lib")
#pragma comment	(lib, "zlib-md.lib")
class TFbxObj : public TModel
{
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
/// <summary>
/// �����Լ� ����Ʈ
/// </summary>
public:
	bool	LoadObject(std::string filename);
	bool	Create(std::wstring vsFile, std::wstring psFile) override;
	bool	PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex) override;
/// <summary>
/// �Ϲ��Լ� ����Ʈ
/// </summary>
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	
};

