#pragma once
#include "TObject3D.h"
#include <fbxsdk.h>

struct TTrack
{
	UINT	iFrame;
	TMatrix matTrack;
};
class TFbxModel : public TObject3D
{
public:	
	int		 m_iIndex = -1;
	TMatrix  m_matLocal;
	TMatrix  m_matAnim;
	FbxNode* m_pFbxParent = nullptr;
	FbxNode* m_pFbxNode = nullptr;
	TFbxModel* m_pParentObj = nullptr;
	std::wstring m_szTexFileName;
	// submaterial
	std::vector<std::wstring>  m_szTexFileList;
	using TSubVertex = std::vector<TVertex>;
	std::vector<TSubVertex>      m_pSubVertexList;
	std::vector<ID3D11Buffer*>   m_pVBList;
	std::vector<TTexture*>		 m_pTextureList;

	std::vector<TTrack>			m_AnimTrack;
public:
	virtual bool    SetVertexData() override;	
	virtual bool	CreateVertexBuffer()override;
	virtual bool    SetIndexData() override;
	virtual bool	PostRender() override;
	virtual bool    Release() override;
	
};
struct TScene
{
	UINT   iStart;
	UINT   iEnd;
	UINT   iFrameSpeed;
};
class TFbxImporter : public TObject3D
{
public:
	TScene		m_Scene;
	float m_fDir = 1.0f;
	float m_fTime = 0.0f;
	float m_fSpeed = 1.0f;
	TBoneWorld	  m_matBoneArray;
public:
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
	FbxNode*		m_pRootNode;
	std::vector<TFbxModel*>  m_DrawList;
	std::vector<TFbxModel*>  m_TreeList;
	ID3D11Buffer* m_pBoneCB = nullptr;
public:
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
	virtual bool	Load(std::string filename);
	virtual void    PreProcess(FbxNode* node, TFbxModel* fbxParent=nullptr);
	virtual void	ParseMesh(TFbxModel* pObject);
	std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);

	void ReadTextureCoord(	FbxMesh* pFbxMesh, 
							FbxLayerElementUV* pUVSet, 
							int vertexIndex, 
							int uvIndex, 
							FbxVector2& uv);
	
	FbxColor ReadColor(const FbxMesh* mesh,
						DWORD dwVertexColorCount,
						FbxLayerElementVertexColor* pVertexColorSet,
						DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh,
		int controlPointIndex,
		int vertexCounter);

	int GetSubMaterialIndex(int iPlygon,FbxLayerElementMaterial* pMtrl);
public:
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
	void		ParseAnimation();
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
	virtual bool	CreateConstantBuffer(ID3D11Device* pDevice);
};

