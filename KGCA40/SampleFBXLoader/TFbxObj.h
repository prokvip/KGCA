#pragma once
#include "TMesh.h"

class TFbxObj 
{
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
public:
	TAnimMatrix     m_matAnimMatrix;
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime=0.0f;
	int		m_iAnimIndex=0;
	float m_fStartTime;
	float m_fEndTime;
	float m_fSampleTime;
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<TMatrix>	m_matBindPoseList;
	std::vector<TMtrl*>		m_pFbxMaterialList;
	std::vector<TMesh*>		m_pMeshList;
	CB_DATA					m_cbData;
/// <summary>
/// 가상함수 리스트
/// </summary>
public:
	int			GetFindInedx(FbxNode* pNode);
	bool		LoadObject( std::string filename, 
							std::string shaderName);
	bool		Frame();
	bool		Release();
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
public:
	void	SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
	bool    Render(ID3D11DeviceContext* pContext);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void	LoadMaterial(TMtrl* pMtrl);
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode, TMesh* pMesh);
	void	ParseMesh(FbxNode* pNode, TMesh* pMesh);	
	TMatrix   ParseTransform(FbxNode* pNode, TMatrix& matParent);
public:
	void	ParseAnimationNode(FbxNode* pNode, TMesh* pMesh);
	void	ParseAnimation();
	void	ParseAnimStack(FbxString* szData);
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, TMesh* pMesh, TSkinData* pSkindata);
public:
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,
		DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);

};

