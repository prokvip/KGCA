#pragma once
#include "TObject2D.h"
class TNpc2D : public TObject2D
{
public:
	bool  Frame() override;
	TNpc2D()
	{
		m_fSpeed = 300.0f;
	}
};

