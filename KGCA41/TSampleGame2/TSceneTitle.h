#pragma once
#include "TScene.h"
#include "TInterface.h"
#include "TProjectile2D.h"
class TSceneTitle :   public TScene
{
public:
	TUser2D*		m_pUser;
	TBaseObject*	m_pMapTitle;
	TInterface*		m_FadeObject;
	TInterface*		m_btnStart;
	TInterface*		m_btnStart2;
	TInterface*		m_listControl;
	TInterface*		m_Dlg;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual bool        IsNextScene();
};
