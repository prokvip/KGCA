#pragma once
#include "TStd.h"
class TObject
{
public:
	int      iNodeIndex;
	XVector2 pos;	
	float   mass;
	XVector2 Velocity; //�ӵ�
	XVector2 Acceleration; //���ӵ�
	XVector2 sumForces; //��� �� �ջ�

	Rect    rect;

public:
	TObject();
};

