#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include "TFsm.h"
class TNpc;
class TPlayer;
class TNpcState
{
public:
	TNpc* m_pOwner = nullptr;
public:
	virtual void Process(TPlayer* player) = 0;
	TNpcState() = delete;
	TNpcState(TNpc* npc)
	{
		m_pOwner = npc;
	}
};
class TStandState : public TNpcState
{
public:
	virtual void Process(TPlayer* player);
	TStandState() = delete;
	TStandState(TNpc* npc) : TNpcState(npc)
	{	
	}
};
class TMoveState : public TNpcState
{
public:
	virtual void Process(TPlayer* player);
	TMoveState() = delete;
	TMoveState(TNpc* npc) : TNpcState(npc)
	{
	}
};
class TAttackState : public TNpcState
{
public:
	virtual void Process(TPlayer* player);
	TAttackState() = delete;
	TAttackState(TNpc* npc) : TNpcState(npc)
	{
	}
};
class TNpc
{
	DWORD m_dwState;
	TFsm* m_pFsm=nullptr;
public:
	TNpcState* m_pCurentState=nullptr;
	std::vector< TNpcState*> m_pActionList;
	void Process(TPlayer* player);
	void SetTransition(DWORD dwEvent);
public:
	TNpc(TFsm* fsm);
	virtual ~TNpc();
};

