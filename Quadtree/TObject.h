#pragma once
#include "TStd.h"
class TObject
{
public:
	int      iNodeIndex;
	TVector2 pos;	
	float   mass;
	TVector2 Velocity; //�ӵ�
	TVector2 Acceleration; //���ӵ�
	TVector2 sumForces; //��� �� �ջ�

	Rect    rect;

public:
	TObject();
};

