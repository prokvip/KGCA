#include "TCollision.h"
TCollisionType TCollision::RectToRect(TRect& a, TRect& b)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // x1(10)----30------x2(40)
    float x1 = a.x;
    float y1 = a.y;
    float x2 = a.x + a.w;
    float y2 = a.y + a.h;

    float z1 = b.x;
    float w1 = b.y;
    float z2 = b.x + b.w;
    float w2 = b.y + b.h;

    // 합집합
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = x1 < z1 ? x1 : z1;
    fMinY = y1 < w1 ? y1 : w1;
    fMaxX = x2 > z2 ? x2 : z2;
    fMaxY = y2 > w2 ? y2 : w2;

    //  가로 판정
    if ((a.w + b.w) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((a.h + b.h) >= (fMaxY - fMinY))
        {
            // 교차한다. 교집합
            return TCollisionType::RECT_OVERLAP;
        }
    }
    return TCollisionType::RECT_OUT;
}

bool    TCollision::RectToInRect(TRect& a, TRect& b)
{
    //  |             |
    if (a.x <= b.x)
    {
        if ((a.x + a.w) >= b.x + b.w)
        {
            if (a.y <= b.y)
            {
                if ((a.y + a.h) >= b.y + b.h)
                {
                    return true;
                }
            }
        }
    }
    return false;
}