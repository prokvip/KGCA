#pragma once
#include "TState.h"
class TStateMove : public TState
{
public:
	virtual void	Process(TObject2D* pPlayer) override;
	TStateMove(TObjectAI* owner) : TState(owner)
	{

	}
};

