#pragma once
#include "TModel.h"
#include <fbxsdk.h>
class TFbxObj : public TModel
{
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
/// <summary>
/// 가상함수 리스트
/// </summary>
public:
	bool	LoadObject(std::string filename);
	bool	Create(std::wstring vsFile, std::wstring psFile) override;
	bool	PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex) override;
/// <summary>
/// 일반함수 리스트
/// </summary>
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	
};

