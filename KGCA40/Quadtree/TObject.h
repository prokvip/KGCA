#pragma once
#include "TStd.h"
class TObject
{
public:
	int      iNodeIndex;
	TVector2 pos;	
	float   mass;
	TVector2 Velocity; //속도
	TVector2 Acceleration; //가속도
	TVector2 sumForces; //모든 힘 합산

	Rect    rect;

public:
	TObject();
};

