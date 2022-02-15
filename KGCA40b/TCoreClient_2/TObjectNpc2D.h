#pragma once
#include "TObject2D.h"
class TObjectNpc2D :   public TObject2D
{
public:
	bool Frame() override;
	TObjectNpc2D();
	virtual ~TObjectNpc2D();
};

