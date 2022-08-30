#pragma once
#include "TObject.h"
class TEnemy : public TObject
{
public:
	bool   Frame(float fDeltaTime, float fGameTime) override;
	bool   Render() override;
	TEnemy();
	TEnemy(std::string name);
};
class TEnemy2D : public TObject2D
{
public:
	bool   Frame(float fDeltaTime, float fGameTime) override;
	bool   Render() override;
	void   SetPosition(TVector2D p, TVector2D s)override;
	TEnemy2D();
	TEnemy2D(std::string name);
};

