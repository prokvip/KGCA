#include "TCollision.h"
TCollisionType   TCollision::BoxToBox(TBox& a, TBox& b)
{
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
    return false;
}