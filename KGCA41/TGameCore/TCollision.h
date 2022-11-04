#pragma once
#include "TMath.h"
#include "TVector.h"
#include "TMatrix.h"
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
        x2 = this->x1 + this->w;
        y2 = this->y1 + this->h;
        cx = (this->x1 + x2) / 2.0f;
        cy = (this->y1 + y2) / 2.0f;
    }
};

struct TSphere
{
    TVector3 vCenter;
    float fRadius;
    TSphere() {};
    TSphere(TVector3 vC,  float r) {
        vCenter = vC;
        fRadius = r;
    };
};
struct Box
{
    TVector3 vMin;
    TVector3 vSize;
};
struct TBox : Box
{
    TVector3 vMax;
    TVector3 vCenter;
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
    TBox(TVector3 vPos,TVector3 vSize) 
    {
        Set(vPos, vSize);
    }
    void  Set(TVector3 vPos,TVector3 vSize)
    {
        vMin = vPos;
        this->vSize = vSize;
        vMax = vMin + vSize;
        vCenter = (vMin + vMax);
        vCenter /= 2.0f;
    }
};

struct T_AABB
{
    TVector3 vMin;
    TVector3 vMax;
};
struct T_OBB
{
    TVector3 vCenter;
    TVector3 vAxis[3];
    float   fDistance[3];
};
struct T_BOX
{
    TVector3 vPos[8];
    // aabb
    TVector3 vMin;
    TVector3 vMax;
    // obb    
    TVector3 vCenter;
    TVector3 vAxis[3];
    float   fExtent[3];
};
class TCollision
{
public:
    static TCollisionType   RectToRect(TRect& a, TRect& b, TRect& Intersect);
    static bool             RectToInRect(TRect& a, TRect& b);
    static bool             RectToPoint(TRect& a, POINT& p);
    static bool             CircleToCircle(TCircle& a, TCircle& b);
public:
    static TCollisionType   BoxToBox(TBox& a, TBox& b);
    static bool             BoxToInBox(TBox& a, TBox& b);
    static bool             SphereToSphere(TSphere& a, TSphere& b);
};
