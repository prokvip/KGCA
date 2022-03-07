#pragma once
#include "TState.h"
class TStateStop : public TState
{
public:
	virtual void	Process(TObject2D* pPlayer) override;
	TStateStop(TObjectAI* owner) : TState(owner)
	{

	}
};

