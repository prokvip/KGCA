#pragma once
#include "TObject2D.h"
class TMapObject : public TObject2D
{
public:
	bool   Frame() override;
	void  UpdateVertexBuffer() override;
};

