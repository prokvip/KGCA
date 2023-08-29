#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TScene.h"
class Sample : public TCore
{
	TScene* m_pScene=nullptr;
	std::shared_ptr<TScene> m_pTitleScene = nullptr;	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

