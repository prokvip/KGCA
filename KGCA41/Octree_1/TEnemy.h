#pragma once
#include "TObject.h"
class TEnemy : public TObject
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
	void   Render() override;
	TEnemy();
	TEnemy(std::string name);
};
class TEnemy2D : public TObject2D
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
	void   Render() override;
	TEnemy2D();
	TEnemy2D(std::string name);
};

