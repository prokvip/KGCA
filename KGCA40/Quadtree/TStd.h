#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "XVector.h"
using namespace std;


struct Rect
{
	float  x, y, w, h;
};
struct TRect
{
	XVector2  p0;
	XVector2  p1;
	XVector2  wh;
	// x1,y1(p0)
	//
	//          x2, y2(p1)
};
