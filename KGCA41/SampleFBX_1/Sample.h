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
class Sample : public TCore
{
	TObject			m_BoxObj;
	TFbxImport		m_pFbx;
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

