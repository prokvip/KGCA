#pragma once
#include "TCore.h"
#include "TSpriteObj.h"

class Sample : public TCore
{
	TObject* m_pMapObj = nullptr;	
	TSpriteObj* m_pSpriteTexObj = nullptr;
	TSpriteObj* m_pSpriteUVObj = nullptr;
	TSpriteObj* m_pSpriteAirObj = nullptr;	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

