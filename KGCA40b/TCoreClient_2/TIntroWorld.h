#pragma once
#include "TWorld.h"
#include "TPlayerObj2D.h"
#include "TObjectNpc2D.h"
#include "TUIObject.h"
#include "TSoundMgr.h"
#include "TShaderMgr.h"
#include "TTextureMgr.h"
class TIntroWorld :   public TWorld
{
public:
	TObject2D			m_PlayerObj;	
	TSound*				m_pBackGroundMusic;
public:
	bool	Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

