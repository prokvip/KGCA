#pragma once
#include "TObject2D.h"
class TPlayerObj2D :   public TObject2D
{
public:
	bool	Frame() override;
	TPlayerObj2D();
	virtual ~TPlayerObj2D();
};

