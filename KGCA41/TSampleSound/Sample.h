#pragma once
#include "TGameCore.h"
#include "TSound.h"

class Sample : public TGameCore
{
	TSound				m_Sound;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

