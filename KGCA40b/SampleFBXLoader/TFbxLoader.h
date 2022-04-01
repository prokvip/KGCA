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
	void ReadTextureCoord(	FbxMesh* pFbxMesh, 
							FbxLayerElementUV* pUVSet, 
							int vertexIndex, 
							int uvIndex, 
							FbxVector2& uv);
	std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

