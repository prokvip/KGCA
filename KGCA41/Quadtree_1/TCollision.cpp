#include "TCollision.h"
TCollisionType TCollision::RectToRect(TRect& a, TRect& b)
{
    // 0 : ��������(0)
    // 1 : ��������(1) -> ������ �ִ� ����(2)
    // a.x(10)----30------a.x2(40)
    // ������
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = a.x < b.x ? a.x : b.x;
    fMinY = a.y < b.y ? a.y : b.y;
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
            TRect rt;
            x = a.x > b.x ? a.x: b.y;
            y = a.y > b.y ? a.y : b.y;
            x2 = a.x2 < b.x2 ? a.x2 : b.x2;
            y2 = a.y2 < b.y2 ? a.y2 : b.y2;
            rt.Set(x, y, x2-x, y2-y);
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