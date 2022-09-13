#pragma once
#include "TScene.h"
class TSceneTitle :   public TScene
{
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
};

