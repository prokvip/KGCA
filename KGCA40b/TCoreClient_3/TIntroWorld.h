#pragma once
#include "TWorld.h"
#include "TPlayerObj2D.h"
#include "TObjectNpc2D.h"
#include "TUIObject.h"
#include "TSoundMgr.h"
#include "TShaderMgr.h"
#include "TTextureMgr.h"
#include "TEffectObj.h"
class TIntroWorld :   public TWorld
{
public:
	//TSprite				m_SpriteObj;
	TEffectObj			m_ExplosionObj;
	TSound*				m_pBackGroundMusic;
	TTexture*			m_pChangeColorTex[10];
	TTexture*			m_pColorTex;
	TWorld*				m_pNextWorld;
public:
	bool	Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

