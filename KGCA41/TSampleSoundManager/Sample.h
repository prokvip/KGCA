#pragma once
#include "TGameCore.h"
#include "TSoundManager.h"

class Sample : public TGameCore
{
	TSound*				m_pBGSound=nullptr;
	TSound*				m_pEffectSound = nullptr;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

