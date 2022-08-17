#pragma once
#include "TObject.h"
class TEnemy : public TObject
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};

