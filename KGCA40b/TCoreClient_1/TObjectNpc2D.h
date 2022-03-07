#pragma once
#include "TObject2D.h"
#include "TStateMove.h"
#include "TStateStop.h"
#include "TStateAttack.h"

class TObjectNpc2D :   public TObjectAI
{
public:
	bool Init();
	bool Frame() override;
	void SetTransition(DWORD dwEvent) override;
};

