#pragma once
#include "TCore.h"
class Sample :   public TCore
{
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

