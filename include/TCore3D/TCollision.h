#pragma once
#include "TMath.h"
enum TCollisionResult
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
// È­¸éÁÂÇ¥°è+¿ÞÂÊ»ó´ÜÀÌ ¿øÁ¡
struct TRect2D
{
	T::TVector2 vMin;
	T::TVector2 vMax;
	T::TVector2 vCenter;
	T::TVector2 size;
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
	TRect2D(T::TVector2 vMin, T::TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect2D(T::TVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + T::TVector2(w, h);
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
// È­¸éÁÂÇ¥°è+Áß¾ÓÀÌ ¿øÁ¡
struct TRect
{
	T::TVector2 vMin;
	T::TVector2 vMax;
	T::TVector2 vCenter;
	T::TVector2 size;
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
	TRect(T::TVector2 vMin, T::TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vCenter = (vMax + vMin);
		vCenter/= 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect(T::TVector2 pos, float w, float h)
	{
		vCenter = pos;
		this->vMin = vCenter - T::TVector2(w/2.0f, h/2.0f);
		this->vMax = vCenter + T::TVector2(w / 2.0f, h / 2.0f);		
		this->size.x = w;
		this->size.y = h;
	}
};
struct TBox
{
	T::TVector3 vList[8];
	T::TVector3 vMin;
	T::TVector3 vMax;
	T::TVector3 vCenter;
	T::TVector3 vAxis[3];
	T::TVector3 size;
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
	TBox(T::TVector3 vMin, T::TVector3 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vCenter = (vMax + vMin);
		vCenter/= 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TBox(T::TVector3 v, float w, float h, float q)
	{
		this->vMin = v;
		this->vMax = vMin + T::TVector3(w, h,q);
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
struct TSphere
{
	T::TVector3 vCenter;
	float    fRadius;
	TSphere()
	{
		fRadius = 3.0f;
	}
};
class TCollision
{
public:
	static bool   SphereToPoint(TSphere rt, float x, float y, float z);
	static bool   SphereToPoint(TSphere rt, T::TVector3 v);
	static bool   RectToPoint(TRect rt, int x, int y);
	static bool   RectToPoint(TRect rt, T::TVector2 v);
	static TCollisionResult    RectToRect(TRect,TRect);
	static TCollisionResult    ToRect(TRect rt1, TRect rt2);
	static TRect  UnionRect(TRect rt1, TRect rt2);
	static bool  IntersectRect(TRect rt1, TRect rt2, TRect* rt);
	
	static bool   BoxToPoint(TBox rt, int x, int y, int z);
	static bool   BoxToPoint(TBox rt, T::TVector3 v);
	static TCollisionResult    BoxToBox(TBox, TBox);
	static TBox   UnionBox(TBox rt1, TBox rt2);
	static bool  IntersectBox(TBox rt1, TBox rt2, TBox* rt);
};

