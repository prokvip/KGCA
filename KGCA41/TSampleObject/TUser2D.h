#pragma once
#include "TObject2D.h"
class TUser2D : public TObject2D
{
public:
	bool  Frame() override;
	TUser2D()
	{
		m_fSpeed = 400.0f;
	}
};
