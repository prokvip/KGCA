#pragma once
#include "TGameCore.h"
class Sample : public TGameCore
{
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

