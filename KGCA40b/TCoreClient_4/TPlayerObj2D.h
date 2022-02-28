#pragma once
#include "TObject2D.h"
class TPlayerObj2D :  public TObject2D
{
public:
	virtual void	HitOverlap(TBaseObject* pObj, DWORD dwState);
	bool	Frame() override;
public:
	TPlayerObj2D();
	virtual ~TPlayerObj2D();
};

