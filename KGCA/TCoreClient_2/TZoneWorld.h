#pragma once
#include "TWorld.h"
#include "TPlayerObj2D.h"
#include "TObjectNpc2D.h"
#include "TSoundMgr.h"
class TZoneWorld :   public TWorld
{
public:
	TPlayerObj2D		m_PlayerObj;
	TSound* m_pBackGroundMusic;
public:
	std::vector<TObjectNpc2D*> m_NpcLlist;
public:
	bool	Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

