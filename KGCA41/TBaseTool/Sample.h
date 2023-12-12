#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TDebugCamera.h"
#include "TQuadtree.h"
#include "TMapObj.h"

class Sample : public TCore
{
public:
	using NEW_OBJECT = std::shared_ptr<TMapObj>;
	std::vector<NEW_OBJECT>		m_MapObj;

	const TTexture* m_pTex;
	std::shared_ptr<TDebugCamera>  m_pDebugCamera;	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool  DeleteDxResource() override;
	bool  CreateDxResource() override;
};

