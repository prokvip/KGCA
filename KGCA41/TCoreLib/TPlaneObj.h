#pragma once
#include "TObject.h"
class TPlaneObj : public TObject
{
public:
	virtual bool	CreateVertexBuffer() override;
	virtual bool    Frame();
};

