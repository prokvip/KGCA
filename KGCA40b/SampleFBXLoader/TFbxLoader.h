#pragma once
#include "TObject3D.h"
#include <fbxsdk.h>
class TFbxObj : public TObject3D
{
public:
	FbxNode* m_pFbxParent = nullptr;
	FbxNode* m_pFbxNode = nullptr;
	TFbxObj* m_pParentObj = nullptr;
	std::wstring m_szTexFileName;
public:
	bool    SetIndexData()
	{
			return true;
	}
};
class TFbxLoader 
{
public:
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
	FbxNode*		m_pRootNode;
	std::vector<TFbxObj*>  m_ObjList;
public:
	virtual bool	Load(std::string filename);
	virtual void    PreProcess(FbxNode* node, FbxNode* parent);
	virtual void	ParseMesh(TFbxObj* pObject);
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
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

