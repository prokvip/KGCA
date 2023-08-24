#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
class Sample : public TCore
{
	TObject* m_pMapObj = nullptr;
	TObject* m_pSpriteObj = nullptr;
	std::vector<const TTexture*>  m_pTexList;
	int  m_iTexIndex=0;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

