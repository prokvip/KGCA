#pragma once
#include "TCore.h"
#include "TMap.h"
class Sample : public TCore
{
	TMap* m_pMapObj = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

