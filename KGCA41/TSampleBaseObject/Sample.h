#pragma once
#include "TGameCore.h"
#include "TObjectPlane.h"

class Sample : public TGameCore
{
	TObjectPlane		m_Plane;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

