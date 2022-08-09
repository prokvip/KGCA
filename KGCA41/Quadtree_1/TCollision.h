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
struct TVector2D
{
    float x;
    float y;
};
struct TCircle
{
    float cx;
    float cy;
    float fRadius;
    TCircle() {};
    TCircle(float x, float y, float r) {
        cx = x;
        cy = y;
        fRadius = r;
    };
};
struct Rect
{
    float  x1;
    float  y1;
    float  w;
    float  h;
};
struct TRect : Rect
{    
    float  x2;//x+w
    float  y2;//y+h
    float  cx;
    float  cy;
    bool   operator == (TRect& dest)
    {
        if (fabs(x1-dest.x1) < 0.001f)
        {
            if (fabs(y1 - dest.y1) < 0.001f)
            {
                if (fabs(w - dest.w) < 0.001f)
                {
                    if (fabs(h - dest.h) < 0.001f)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    TRect(){}
    TRect(float x, float y, float z, float w) {
        Set(x, y, z, w);
    }
    void  Set(float x, float y, float z, float w)
    {
        this->x1 = x;
        this->y1 = y;
        this->w = z;
        this->h = w;
        x2 = x + w;
        y2 = y + h;
        cx = (x + x1) / 2.0f;
        cy = (y + y2) / 2.0f;
    }
};

class TCollision
{
public:
    static TCollisionType   RectToRect(TRect& a, TRect& b);
    static bool             RectToInRect(TRect& a, TRect& b);
    static bool             CircleToCircle(TCircle& a, TCircle& b);
};
