#pragma once
#include "TScene.h"
#include "TInterface.h"
#include "TProjectile2D.h"
#include "TObject3D.h"
#include "TCameraDebug.h"
#include "TMap.h"
class TSceneTitle :   public TScene
{
public:
	TUser2D*		m_pUser;
	TBaseObject*	m_pBG;
	TMap* m_pMap = nullptr;
	TObject3D* m_pBoxObjA=nullptr;
	TObject3D* m_pBoxObjB = nullptr;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual bool        IsNextScene();
	
};

