#pragma once
#include "TEffect2D.h"
class TProjectile2D : public TEffect2D
{
public:
	virtual bool Update() override;
	TProjectile2D();
};

