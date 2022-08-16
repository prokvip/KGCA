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
    static TCollisionType   BoxToBox(TBox& a, TBox& b);
    static bool             BoxToInBox(TBox& a, TBox& b);
    static bool             SphereToSphere(TSphere& a, TSphere& b);
};
