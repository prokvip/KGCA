#pragma once
#include "TScene.h"
class TSceneTitle :   public TScene
{
public:
	TUser2D* m_pUser;
	TBaseObject* m_pMapTitle;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
};

