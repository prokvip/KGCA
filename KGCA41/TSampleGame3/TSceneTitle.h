#pragma once
#include "TScene.h"
#include "TInterface.h"
#include "TProjectile2D.h"
#include "TShape.h"
#include "TCamera.h"
#include "TMap.h"
class TSceneTitle :   public TScene
{
public:
	TUser2D*		m_pUser;
	TBaseObject*	m_pBG;
	TMap* m_pMap = nullptr;
	TShapeBox* m_pBoxObjA=nullptr;
	TShapeBox* m_pBoxObjB = nullptr;
	TCamera* m_pMainCamera = nullptr;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual bool        IsNextScene();
};

