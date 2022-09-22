#pragma once
#include "TGameCore.h"
#include "TSceneTitle.h"
#include "TSceneInGame.h"

class Sample : public TGameCore
{
public:
	TSceneTitle*	m_pTitle = nullptr;
	TSceneInGame*	m_pInGame = nullptr;
	TScene*			m_pCurrentScene = nullptr;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

