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
struct Rect
{
    float  x;
    float  y;
    float  w;
    float  h;
};
struct TRect : Rect
{    
    float  x2;//x+w
    float  y2;//y+h
    float  cx;
    float  cy;
    TRect(){}
    TRect(float x, float y, float z, float w) {
        Set(x, y, z, w);
    }
    void  Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->w = z;
        this->h = w;
        x2 = x + w;
        y2 = y + h;
    }
};

class TCollision
{
public:
    static TCollisionType   RectToRect(TRect& a, TRect& b);
    static bool             RectToInRect(TRect& a, TRect& b);
};
