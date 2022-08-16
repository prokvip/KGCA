#pragma once
#include "TObject2D.h"
class TGamePlayer2D : public TObject2D
{
public:
	void	Frame(float fDeltaTime, float fGameTime) override;
};

