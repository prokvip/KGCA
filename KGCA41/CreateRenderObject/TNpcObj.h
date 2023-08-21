#pragma once
#include "TPlaneObj.h"
class TNpcObj : public TPlaneObj
{
	TVector3 m_vDirection = { 1,1,0 };
public:
	void			Move(float fSecond);
	virtual bool    Frame();
	TNpcObj();
	virtual ~TNpcObj();
};
