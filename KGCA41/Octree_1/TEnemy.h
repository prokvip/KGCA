#pragma once
#include "TObject.h"
class TEnemy : public TObject
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};
class TEnemy2D : public TObject2D
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};

