#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TDebugCamera.h"
#include "TQuadtree.h"
#include "TFbxImport.h"

class TMapObject : public TObject
{
public:
	virtual bool  CreateVertexData()
	{
		return true;
	}
	virtual bool  CreateIndexData()
	{
		return true;
	}
};
using NEW_OBJECT = std::shared_ptr<TObject>;
using NEW_FBX = std::shared_ptr<TFbxImport>;

class Sample : public TCore
{
	//std::vector<NEW_OBJECT>		m_BoxObj;
	NEW_OBJECT					m_BoxObj;
	std::vector<NEW_FBX>		m_pFbxList;

	const TTexture* m_pTex;
	TMap* m_pMapObj = nullptr;
	std::shared_ptr<TDebugCamera>  m_pDebugCamera;
	std::shared_ptr< TQuadtree> m_Quadtree;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool  DeleteDxResource() override;
	bool  CreateDxResource() override;
};

