#pragma once
#include "TScene.h"
#include "TInterface.h"
#include "TProjectile2D.h"
#include "TShape.h"
class TSceneTitle :   public TScene
{
public:
	TUser2D*		m_pUser;
	TBaseObject*	m_pBG;
	TShapeBox* m_pBoxObj=nullptr;

public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual bool        IsNextScene();
};

