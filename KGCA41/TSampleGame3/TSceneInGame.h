#pragma once
#include "TScene.h"
#include "TInterface.h"
#include "TProjectile2D.h"

class TSceneInGame : public TScene
{
public:
	TVector2D		m_vCamera;
	TMapObject*		m_pMap;
	TUser2D*		m_pUser;
	std::list<TEffect2D*>		m_pEffectList;
	std::list<TNpc2D*>		m_pNpcList;
	std::list<TProjectile2D*>  m_Projectile;
public:
	void AddProjectile(TVector2D pos);
	void AddEffect();
	void AddEffect(float x, float y);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual void		DrawMiniMap(UINT x, UINT y, UINT w=100, UINT h=100);
};

