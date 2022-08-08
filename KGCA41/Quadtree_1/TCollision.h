#pragma once
#include <iostream>
#include <vector>
#include <queue>

enum TCollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};

struct TRect
{
    float  x;
    float  y;
    float  w;
    float  h;
};

class TCollision
{
public:
    static TCollisionType RectToRect(TRect& a, TRect& b);
};
TCollisionType TCollision::RectToRect(TRect& a, TRect& b)
{
    // 0 : ��������(0)
    // 1 : ��������(1) -> ������ �ִ� ����(2)
    // x1(10)----30------x2(40)
    float x1 = a.x;
    float y1 = a.y;
    float x2 = a.x + a.w;
    float y2 = a.y + a.h;

    float z1 = b.x;
    float w1 = b.y;
    float z2 = b.x + b.w;
    float w2 = b.y + b.h;

    // ������
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = x1 < z1 ? x1 : z1;
    fMinY = y1 < w1 ? y1 : w1;
    fMaxX = x2 > z2 ? x2 : z2;
    fMaxY = y2 > w2 ? y2 : w2;

    //  ���� ����
    if ((a.w + b.w) >= (fMaxX - fMinX))
    {
        //  ���� ����
        if ((a.h + b.h) >= (fMaxY - fMinY))
        {
            // �����Ѵ�. ������
            return TCollisionType::RECT_OVERLAP;
        }
    }
    return TCollisionType::RECT_OUT;
}