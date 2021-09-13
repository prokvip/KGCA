#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "TVector.h"
using namespace std;


struct Rect
{
	float  x, y, w, h;
};
struct TRect
{
	TVector2  p0;
	TVector2  p1;
	TVector2  wh;
	// x1,y1(p0)
	//
	//          x2, y2(p1)
};
