#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>

using namespace std;

struct TVector2
{
	float x;
	float y;
	TVector2  operator + (TVector2 p)
	{
		TVector2 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	TVector2() {}
	TVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
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