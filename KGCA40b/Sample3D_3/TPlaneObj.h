#pragma once
#include "TObject3D.h"
class TPlaneObj :   public TObject3D
{
public:
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
public:
	TPlaneObj();
	virtual ~TPlaneObj();
};

