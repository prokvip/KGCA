#pragma once
#include "TCore.h"
class Sample : public TCore
{
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
public:
	Sample();
	~Sample();
};

