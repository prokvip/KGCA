#pragma once
#include "TStd.h"
class TObject
{
public:
	int      iNodeIndex;
	XVector2 pos;	
	float   mass;
	XVector2 Velocity; //속도
	XVector2 Acceleration; //가속도
	XVector2 sumForces; //모든 힘 합산

	Rect    rect;

public:
	TObject();
};

