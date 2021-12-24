#pragma once
#include "TVector2.h"
struct TRect
{
	TVector2 vMin;
	TVector2 vMax;
	TVector2 middle;
	TVector2 size;
	bool operator == (const TRect& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	TRect() {};
	TRect(TVector2 vMin, TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		middle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect(TVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + TVector2(w, h);
		middle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
class TCollision
{
public:
	static bool   RectToPoint(TRect rt, int x, int y);
	static bool   RectToPoint(TRect rt, TVector2 v);
	static int    RectToRect(TRect,TRect);
	static TRect  UnionRect(TRect rt1, TRect rt2);
	static bool  IntersectRect(TRect rt1, TRect rt2, TRect* rt);
};

