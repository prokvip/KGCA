#pragma once
#include "TStd.h"
class TCollision
{
public:
	static bool  LineToPoint(Rect desk, Rect src) { return true; };
	static bool  LineToSphere(Rect desk, Rect src) { return true; };
	static bool  RectToRect(Rect desk, Rect src) { return true; };
	static bool  RectToPoint(Rect desk, TVector pos) { return true; };
};

