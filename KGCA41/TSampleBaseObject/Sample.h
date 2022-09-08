#pragma once
#include "TGameCore.h"
#include "TBaseObject.h"

class Sample : public TGameCore
{
	TBaseObject			m_Object;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

