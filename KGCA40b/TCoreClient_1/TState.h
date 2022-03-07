#pragma once
#include "TObject2D.h"
class TState;
enum T_EnemyState
{
	STATE_STOP= 0,
	STATE_MOVE,
	STATE_ATTACK,
};
enum T_EnemyEvent
{
	STATE_STOP_TIMER = 0,
	STATE_MOVE_TIMER,
	STATE_FIND_TARGET,
	STATE_LOST_TARGET,
};
class TObjectAI : public TObject2D
{
public:
	TState* m_pStateArray[3];
	TState* m_pStateAction = nullptr;
	DWORD   m_dwStateAction;
public:
	float m_fMoveTimer = 0.0f;
	float m_fStopTimer = 0.0f;
public:
	virtual void SetTransition(DWORD dwEvent) = 0;
};
class TState
{
public:
	TObjectAI* m_pOwner;
	DWORD	   m_dwStateAI;
public:
	virtual void	Process(TObject2D* pPlayer) = 0;
	TState(TObjectAI* owner) : m_pOwner(owner)
	{

	}
};

