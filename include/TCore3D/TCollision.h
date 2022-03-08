#pragma once
#include "TVector3.h"
enum TCollisionResult
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
// È­¸éÁÂÇ¥°è+¿ÞÂÊ»ó´ÜÀÌ ¿øÁ¡
struct TRect2D
{
	TVector2 vMin;
	TVector2 vMax;
	TVector2 vMiddle;
	TVector2 size;
	bool operator == (const TRect2D& v)
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
	TRect2D() {};
	TRect2D(TVector2 vMin, TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect2D(TVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + TVector2(w, h);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
// È­¸éÁÂÇ¥°è+Áß¾ÓÀÌ ¿øÁ¡
struct TRect
{
	TVector2 vMin;
	TVector2 vMax;
	TVector2 vMiddle;
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
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect(TVector2 pos, float w, float h)
	{
		vMiddle = pos;
		this->vMin = vMiddle - TVector2(w/2.0f, h/2.0f);
		this->vMax = vMiddle + TVector2(w / 2.0f, h / 2.0f);		
		this->size.x = w;
		this->size.y = h;
	}
};
struct TBox
{
	TVector3 vMin;
	TVector3 vMax;
	TVector3 vMiddle;
	TVector3 size;
	bool operator == (const TBox& v)
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
	TBox() {};
	TBox(TVector3 vMin, TVector3 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TBox(TVector3 v, float w, float h, float q)
	{
		this->vMin = v;
		this->vMax = vMin + TVector3(w, h,q);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
struct TSphere
{
	TVector2 vCenter;
	float    fRadius;
	TSphere()
	{
		fRadius = 3.0f;
	}
};
class TCollision
{
public:
	static bool   SphereToPoint(TSphere rt, int x, int y);
	static bool   SphereToPoint(TSphere rt, TVector2 v);
	static bool   RectToPoint(TRect rt, int x, int y);
	static bool   RectToPoint(TRect rt, TVector2 v);
	static TCollisionResult    RectToRect(TRect,TRect);
	static TCollisionResult    ToRect(TRect rt1, TRect rt2);
	static TRect  UnionRect(TRect rt1, TRect rt2);
	static bool  IntersectRect(TRect rt1, TRect rt2, TRect* rt);
	
	static bool   BoxToPoint(TBox rt, int x, int y, int z);
	static bool   BoxToPoint(TBox rt, TVector3 v);
	static TCollisionResult    BoxToBox(TBox, TBox);
	static TBox   UnionBox(TBox rt1, TBox rt2);
	static bool  IntersectBox(TBox rt1, TBox rt2, TBox* rt);
};

