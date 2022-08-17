#include "TCollision.h"
TCollisionType   TCollision::BoxToBox(TBox& a, TBox& b)
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

    //  가로 판정
    if ((a.vSize.x + b.vSize.x) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((a.vSize.y + b.vSize.y) >= (fMaxY - fMinY))
        {
            if ((a.vSize.z + b.vSize.z) >= (fMaxZ - fMinZ))
            {
                // 교차한다. 교집합
                TVector vMin, vMax;
                TBox Intersect;
                vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
                vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;
                vMin.z = a.vMin.z > b.vMin.z ? a.vMin.z : b.vMin.z;

                vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
                vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;
                vMax.z = a.vMax.z < b.vMax.z ? a.vMax.z : b.vMax.z;

                Intersect.Set(vMin, vMax-vMin);
                if (Intersect == a || Intersect == b)
                {
                    return TCollisionType::RECT_IN;
                }
                return TCollisionType::RECT_OVERLAP;
            }
        }
    }
    return TCollisionType::RECT_OUT;
}
bool   TCollision::BoxToInBox(TBox& a, TBox& b)
{
    //  |             |
    if (a.vMin <= b.vMin)
    {
        if ( a.vMax >= b.vMax)
        {
            return true;
        }
    }
    return false;
}
bool             TCollision::SphereToSphere(TSphere& a, TSphere& b)
{
    float fSumRadius = a.fRadius + b.fRadius;
    TVector vDir = a.vCenter - b.vCenter;    
    float fDistance = vDir.Length();
    if (fDistance <= fSumRadius)
    {
        return true;
    }
    return false;
}