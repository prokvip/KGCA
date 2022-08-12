#pragma once
#include "TObject.h"
class TNonCharacter : public TObject
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};

