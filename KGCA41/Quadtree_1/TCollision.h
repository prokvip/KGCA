#pragma once
#include <iostream>
#include <vector>
#include <queue>

enum TCollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};

struct TRect
{
    float  x;
    float  y;
    float  w;
    float  h;
};

class TCollision
{
public:
    static TCollisionType   RectToRect(TRect& a, TRect& b);
    static bool             RectToInRect(TRect& a, TRect& b);
};
