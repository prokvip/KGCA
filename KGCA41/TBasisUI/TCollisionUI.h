#pragma once
#include "TVectorUI.h"
#include <vector>
#include <queue>
namespace TUI
{
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
        TVector2D operator += (TVector2D vAdd)
        {           
            Set(x1+vAdd.x, y1+vAdd.y, w, h);
            return { cx,cy };
        }
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
        TRect() 
        { 
            Set(x1, y1, w, h); 
        }
        TRect(float x, float y, float w, float h) {
            Set(x, y, w, h);
        }
        void  Set(float x, float y, float w, float h)
        {
            this->x1 = x;
            this->y1 = y;
            this->w = w;
            this->h = h;
            x2 = this->x1 + this->w;
            y2 = this->y1 + this->h;
            cx = (this->x1 + this->x2) / 2.0f;
            cy = (this->y1 + this->y2) / 2.0f;
        }
        void  Add(TVector2D pt)
        {
            this->x1 += pt.x;
            this->y1 += pt.y;
            this->x2 += pt.x;
            this->y2 += pt.y;
            x2 = this->x1 + this->w;
            y2 = this->y1 + this->h;
            cx = (this->x1 + this->x2) / 2.0f;
            cy = (this->y1 + this->y2) / 2.0f;
        }
    };

    struct TSphere
    {
        TVector vCenter;
        float fRadius;
        TSphere() {};
        TSphere(TVector vC, float r) {
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
            if (vMin == dest.vMin)
            {
                if (vSize == dest.vSize)
                {
                    return true;
                }
            }
            return false;
        }
        TBox() {}
        TBox(TVector vPos, TVector vSize)
        {
            Set(vPos, vSize);
        }
        void  Set(TVector vPos, TVector vSize)
        {
            vMin = vPos;
            this->vSize = vSize;
            vMax = vMin + vSize;
            vCenter = (vMin + vMax) / 2.0f;
        }
    };

    class TBASISUI_API TCollision
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
};