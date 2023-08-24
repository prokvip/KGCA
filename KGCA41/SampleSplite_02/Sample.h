#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
class Sample : public TCore
{
	TObject* m_pMapObj = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

