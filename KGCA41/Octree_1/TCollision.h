#pragma once
#include "TVector.h"
#include <vector>
#include <queue>

enum TCollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
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
        if (fabs(x1 - dest.x1) < T_Epsilon)
        {
            if (fabs(y1 - dest.y1) < T_Epsilon)
            {
                if (fabs(w - dest.w) < T_Epsilon)
                {
                    if (fabs(h - dest.h) < T_Epsilon)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    TRect() {}
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
        cx = (x + x2) / 2.0f;
        cy = (y + y2) / 2.0f;
    }
};

struct TSphere
{
    TVector vCenter;
    float fRadius;
    TSphere() {};
    TSphere(TVector vC,  float r) {
        vCenter = vC;
        fRadius = r;
    };
};
struct Box
{
    TVector vMin;
    TVector vSize;
};
struct TBox : Box
{
    TVector vMax;
    TVector vCenter;
    bool   operator == (TBox& dest)
    {
        if ( vMin == dest.vMin)
        {
            if (vSize == dest.vSize)
            {
               return true;                      
            }          
        }
        return false;
    }
    TBox(){}
    TBox(TVector vPos,TVector vSize) 
    {
        Set(vPos, vSize);
    }
    void  Set(TVector vPos,TVector vSize)
    {
        vMin = vPos;
        this->vSize = vSize;
        vMax = vMin + vSize;
        vCenter = (vMin + vMax) / 2.0f;
    }
};

class TCollision
{
public:
    static TCollisionType   RectToRect(TRect& a, TRect& b);
    static bool             RectToInRect(TRect& a, TRect& b);
    static bool             CircleToCircle(TCircle& a, TCircle& b);
public:
    static TCollisionType   BoxToBox(TBox& a, TBox& b);
    static bool             BoxToInBox(TBox& a, TBox& b);
    static bool             SphereToSphere(TSphere& a, TSphere& b);
};
