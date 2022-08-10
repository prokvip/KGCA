#pragma once
#include <vector>
#include <string>
#include <map>
struct Rect
{
	float  left;
	float  top;
	float  right;	
	float  bottom;
};
struct TVector2D
{
	float  x;
	float  y;
};
class TRect : public Rect
{
public:
	TRect() {};
	TRect(float l, float t, float r, float b) 
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}
};
class TShape
{
};

