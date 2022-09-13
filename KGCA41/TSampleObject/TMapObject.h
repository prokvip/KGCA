#pragma once
#include "TBaseObject.h"
class TMapObject : public TBaseObject
{
	TVector2D           m_vUserPos;
public:
	bool   Frame() override;
	bool   Render() override;
};

