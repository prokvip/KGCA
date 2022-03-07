#pragma once
#include "TState.h"
class TStateAttack :   public TState
{
public:
	virtual void	Process(TObject2D* pPlayer) override;
	TStateAttack(TObjectAI* owner) : TState(owner)
	{

	}
};

