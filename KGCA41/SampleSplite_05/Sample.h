#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TSoundMgr.h"
class Sample : public TCore
{
	TObject* m_pMapObj = nullptr;	
	TSound*   sound;
	TSound*   effectsound;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

