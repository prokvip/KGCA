#pragma once
#include "TObject2D.h"
class TMapObject : public TObject2D
{
	bool   FrameScroll();
public:
	bool   Frame() override;
	void  UpdateVertexBuffer() override;
};

