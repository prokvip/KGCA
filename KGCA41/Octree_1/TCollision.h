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
    float cx;
    float cy;
    float cz;
    float fRadius;
    TSphere() {};
    TSphere(float x, float y, float z,  float r) {
        cx = x;
        cy = y;
        cz = z;
        fRadius = r;
    };
};
struct Box
{
    float  x1;
    float  y1;
    float  z1;
    float  w;
    float  h;
    float  d;
};
struct TBox : Box
{
    float  x2;//x+w
    float  y2;//y+h
    float  z2;//y+h
    float  cx;
    float  cy;
    float  cz;
    bool   operator == (TBox& dest)
    {
        if (fabs(x1-dest.x1) < 0.001f)
        {
            if (fabs(y1 - dest.y1) < 0.001f)
            {
                if (fabs(z1 - dest.z1) < 0.001f)
                {
                    if (fabs(w - dest.w) < 0.001f)
                    {
                        if (fabs(h - dest.h) < 0.001f)
                        {
                            if (fabs(d - dest.d) < 0.001f)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    TBox(){}
    TBox(float x, float y, float z, 
         float w, float h, float d) {
        Set(x, y, z, w,h,d);
    }
    void  Set(float x, float y, float z,
        float w, float h, float d)
    {
        this->x1 = x;
        this->y1 = y;
        this->z1 = z;
        this->w = w;
        this->h = h;
        this->d = d;
        x2 = x + w;
        y2 = y + h;
        z2 = z + d;
        cx = (x + x1) / 2.0f;
        cy = (y + y2) / 2.0f;
        cz = (z + z2) / 2.0f;
    }
};

class TCollision
{
};
