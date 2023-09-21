#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TDebugCamera.h"
#include "TQuadtree.h"
#include "TFbxImport.h"
class Sample : public TCore
{
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

