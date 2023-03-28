#include "TCollisionUI.h"
namespace TUI
{
    bool  TCollision::RectToPoint(TRect& a, POINT& p)
    {
        if (a.x1 <= p.x && a.x2 >= p.x)
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
        // 0 : ��������(0)
        // 1 : ��������(1) -> ������ �ִ� ����(2)
        // a.x(10)----30------a.x2(40)
        // ������
        float fMinX;   float fMinY;
        float fMaxX;   float fMaxY;
        fMinX = a.x1 < b.x1 ? a.x1 : b.x1;
        fMinY = a.y1 < b.y1 ? a.y1 : b.y1;
        fMaxX = a.x2 > b.x2 ? a.x2 : b.x2;
        fMaxY = a.y2 > b.y2 ? a.y2 : b.y2;
        //  ���� ����
        if ((a.w + b.w) >= (fMaxX - fMinX))
        {
            //  ���� ����
            if ((a.h + b.h) >= (fMaxY - fMinY))
            {
                // �����Ѵ�. ������  
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

    TCollisionType   TCollision::BoxToBox(TBox& a, TBox& b)
    {
        // 0 : ��������(0)
        // 1 : ��������(1) -> ������ �ִ� ����(2)
        // a.x(10)----30------a.x2(40)
        // ������
        float fMinX;    float fMaxX;
        float fMinY;    float fMaxY;
        float fMinZ;    float fMaxZ;
        fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
        fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;
        fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;
        fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;

        fMinZ = a.vMin.z < b.vMin.z ? a.vMin.z : b.vMin.z;
        fMaxZ = a.vMax.z > b.vMax.z ? a.vMax.z : b.vMax.z;

        //  ���� ����
        if ((a.vSize.x + b.vSize.x) >= (fMaxX - fMinX))
        {
            //  ���� ����
            if ((a.vSize.y + b.vSize.y) >= (fMaxY - fMinY))
            {
                if ((a.vSize.z + b.vSize.z) >= (fMaxZ - fMinZ))
                {
                    // �����Ѵ�. ������
                    TVector vMin, vMax;
                    TBox Intersect;
                    vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
                    vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;
                    vMin.z = a.vMin.z > b.vMin.z ? a.vMin.z : b.vMin.z;

                    vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
                    vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;
                    vMax.z = a.vMax.z < b.vMax.z ? a.vMax.z : b.vMax.z;

                    Intersect.Set(vMin, vMax - vMin);
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
            if (a.vMax >= b.vMax)
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
};