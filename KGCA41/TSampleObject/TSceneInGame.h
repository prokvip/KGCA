#pragma once
#include "TScene.h"
class TSceneInGame : public TScene
{
public:
	TMapObject* m_pMap;
	std::vector<TNpc2D*>		m_pNpcList;
	std::vector<TTexture*>		m_ObjectTextureList;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual void		DrawMiniMap(UINT x, UINT y, UINT w=100, UINT h=100);
};

