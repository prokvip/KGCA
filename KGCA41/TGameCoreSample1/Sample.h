#pragma once
#include "TDevice.h"
//#define TCORE
class Sample : public TDevice
{
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	virtual bool		Run() override;
};

