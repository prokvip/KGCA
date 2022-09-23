#pragma once
#include "TScene.h"
#include "TInterface.h"
class TSceneTitle :   public TScene
{
public:
	TUser2D*		m_pUser;
	TBaseObject*	m_pMapTitle;
	TInterface*		m_btnStart;
	TListControl*		m_listControl;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual bool        IsNextScene();
};

