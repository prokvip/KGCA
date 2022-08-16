#pragma once
#include "TObject2D.h"
class TNonCharacter2D : public TObject2D
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};

