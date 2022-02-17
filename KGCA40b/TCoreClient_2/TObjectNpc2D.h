#pragma once
#include "TObject2D.h"
class TObjectNpc2D :   public TObject2D
{
public:
	bool Frame() override;
	void HitOverlap(TBaseObject* pObj, DWORD dwState);
	void HitSelect(TBaseObject* pObj, DWORD dwState);
public:
	TObjectNpc2D();
	virtual ~TObjectNpc2D();
};

