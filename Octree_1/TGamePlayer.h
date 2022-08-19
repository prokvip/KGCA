#pragma once
#include "TObject.h"
class TGamePlayer : public TObject
{
public:
	void	Frame(float fDeltaTime, float fGameTime) override;
};
class TGamePlayer2D : public TObject2D
{
public:
	void	Frame(float fDeltaTime, float fGameTime) override;
};
