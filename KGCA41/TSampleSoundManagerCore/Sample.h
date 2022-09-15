#pragma once
#include "TGameCore.h"

class Sample : public TGameCore
{
	TSound*	m_pBGSound=nullptr;
	TSound*	m_pShot= nullptr;
	TSound* m_pEffect = nullptr;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

