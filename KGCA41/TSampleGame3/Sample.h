#pragma once
#include "TGameCore.h"
#include "TSceneTitle.h"
#include "TSceneInGame.h"
#include "TRenderTarget.h"
#include "TQuadtree.h"
class Sample : public TGameCore
{
public:
	std::shared_ptr<TSceneTitle>	m_pTitle = nullptr;
	std::shared_ptr<TSceneInGame>	m_pInGame = nullptr;
	std::shared_ptr<TScene>			m_pCurrentScene = nullptr;
	TShapeDirectionLine						m_DirLine;
	TQuadtree						m_Quadtree;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

