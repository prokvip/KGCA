#pragma once
#include "TGameCore.h"
#include "TWriter.h"

class Sample : public TGameCore
{
	TWriter				m_Writer;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};
