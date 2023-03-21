#include "pch.h"
#include "TCollision.h"
bool  TCollision::RectToPoint(TRect& a, POINT& p)
{
    if (a.x1 <= p.x && a.x2 >= p.x )
    {
        if (a.y1 <= p.y && a.y2 >= p.y)
        {   
            return true;         
        }
    }
    return false;
}
TCollisionType TCollision::RectToRect(TRect& a, TRect& b, TRect& Intersect)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // a.x(10)----30------a.x2(40)
    // 합집합
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = a.x1 < b.x1 ? a.x1 : b.x1;
    fMinY = a.y1 < b.y1 ? a.y1 : b.y1;
    fMaxX = a.x2 > b.x2 ? a.x2 : b.x2;
    fMaxY = a.y2 > b.y2 ? a.y2 : b.y2;
    //  가로 판정
    if ((a.w + b.w) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((a.h + b.h) >= (fMaxY - fMinY))
        {
            // 교차한다. 교집합  
            float x, y, x2, y2;
            x = a.x1 > b.x1 ? a.x1 : b.x1;
            y = a.y1 > b.y1 ? a.y1 : b.y1;
            x2 = a.x2 < b.x2 ? a.x2 : b.x2;
            y2 = a.y2 < b.y2 ? a.y2 : b.y2;
            Intersect.Set(x, y, x2 - x, y2 - y);
            if (Intersect == a || Intersect == b)
            {
                return TCollisionType::RECT_IN;
            }
            
            return TCollisionType::RECT_OVERLAP;
        }
    }
    return TCollisionType::RECT_OUT;
}

bool    TCollision::RectToInRect(TRect& a, TRect& b)
{
    //  |             |
    if (a.x1 <= b.x1)
    {
        if ((a.x1 + a.w) >= b.x1 + b.w)
        {
            if (a.y1 <= b.y1)
            {
                if ((a.y1 + a.h) >= b.y1 + b.h)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool   TCollision::CircleToCircle(TCircle& a, TCircle& b)
{
    float fSumRadius = a.fRadius + b.fRadius;
    float x = a.cx - b.cx;
    float y = a.cy - b.cy;
    float fDistance = sqrt(x * x + y * y);
    if (fDistance <= fSumRadius)
    {
        return true;
    }
    return false;
}

TCollisionType   TCollision::BoxToBox(T_BOX& a, T_BOX& b)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // a.x(10)----30------a.x2(40)
    // 합집합
    float fMinX;    float fMaxX;
    float fMinY;    float fMaxY;
    float fMinZ;    float fMaxZ;
    fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
    fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;
    fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;
    fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;
    
    fMinZ = a.vMin.z < b.vMin.z ? a.vMin.z : b.vMin.z;
    fMaxZ = a.vMax.z > b.vMax.z ? a.vMax.z : b.vMax.z;

    TVector3 vSize;
    vSize.x = (a.vMax.x - a.vMin.x) + (b.vMax.x - b.vMin.x);
    vSize.y = (a.vMax.y - a.vMin.y) + (b.vMax.y - b.vMin.y);
    vSize.z = (a.vMax.z - a.vMin.z) + (b.vMax.z - b.vMin.z);

    //  가로 판정
    if (vSize.x >= (fMaxX - fMinX))
    {        //  세로 판정
        if (vSize.y >= (fMaxY - fMinY))
        {
            if (vSize.z >= (fMaxZ - fMinZ))
            {
                // 교차한다. 교집합
                TVector3 vMin, vMax;
                T_BOX Intersect;
                vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
                vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;
                vMin.z = a.vMin.z > b.vMin.z ? a.vMin.z : b.vMin.z;

                vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
                vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;
                vMax.z = a.vMax.z < b.vMax.z ? a.vMax.z : b.vMax.z;
                
                if (BoxToInBox(a,b))
                {
                    return TCollisionType::RECT_IN;
                }
                return TCollisionType::RECT_OVERLAP;
            }
        }
    }
    return TCollisionType::RECT_OUT;
}
bool   TCollision::BoxToInBox(T_BOX& a, T_BOX& b)
{
    //  |             |
    if (a.vMin.x <= b.vMin.x && 
        a.vMin.y <= b.vMin.y &&
        a.vMin.z <= b.vMin.z)
    {
        if (a.vMax.x >= b.vMax.x &&
            a.vMax.y >= b.vMax.y &&
            a.vMax.z >= b.vMax.z)
        {
            return true;
        }
    }
    return false;
}
bool   TCollision::BoxToPosition(T_BOX& a, TVector3& p)
{
    //  |             |
    if (a.vMin.x <= p.x &&
        a.vMin.y <= p.y &&
        a.vMin.z <= p.z)
    {
        if (a.vMax.x >= p.x &&
            a.vMax.y >= p.y &&
            a.vMax.z >= p.z)
        {
            return true;
        }
    }
    return false;
}
bool             TCollision::SphereToSphere(TSphere& a, TSphere& b)
{
    float fSumRadius = a.fRadius + b.fRadius;
    TVector3 vDir = a.vCenter - b.vCenter;    
    float fDistance = vDir.Length();
    if (fDistance <= fSumRadius)
    {
        return true;
    }
    return false;
}