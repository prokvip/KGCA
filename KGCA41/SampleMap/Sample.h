#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TDebugCamera.h"
class Sample : public TCore
{
	const TTexture* m_pTex;
	TMap* m_pMapObj = nullptr;
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
