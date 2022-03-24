#pragma once
#include "TMatrix.h"
enum TCollisionResult
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
// È­¸éÁÂÇ¥°è+¿ÞÂÊ»ó´ÜÀÌ ¿øÁ¡
struct TRect2D
{
	TMath::TVector2 vMin;
	TMath::TVector2 vMax;
	TMath::TVector2 vMiddle;
	TMath::TVector2 size;
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
	TRect2D(TMath::TVector2 vMin, TMath::TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect2D(TMath::TVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + TMath::TVector2(w, h);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
// È­¸éÁÂÇ¥°è+Áß¾ÓÀÌ ¿øÁ¡
struct TRect
{
	TMath::TVector2 vMin;
	TMath::TVector2 vMax;
	TMath::TVector2 vMiddle;
	TMath::TVector2 size;
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
	TRect(TMath::TVector2 vMin, TMath::TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TRect(TMath::TVector2 pos, float w, float h)
	{
		vMiddle = pos;
		this->vMin = vMiddle - TMath::TVector2(w/2.0f, h/2.0f);
		this->vMax = vMiddle + TMath::TVector2(w / 2.0f, h / 2.0f);		
		this->size.x = w;
		this->size.y = h;
	}
};
struct TBox
{
	TMath::TVector3 vMin;
	TMath::TVector3 vMax;
	TMath::TVector3 vMiddle;
	TMath::TVector3 size;
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
	TBox(TMath::TVector3 vMin, TMath::TVector3 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	TBox(TMath::TVector3 v, float w, float h, float q)
	{
		this->vMin = v;
		this->vMax = vMin + TMath::TVector3(w, h,q);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
struct TSphere
{
	TMath::TVector2 vCenter;
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
	static bool   SphereToPoint(TSphere rt, TMath::TVector2 v);
	static bool   RectToPoint(TRect rt, int x, int y);
	static bool   RectToPoint(TRect rt, TMath::TVector2 v);
	static TCollisionResult    RectToRect(TRect,TRect);
	static TCollisionResult    ToRect(TRect rt1, TRect rt2);
	static TRect  UnionRect(TRect rt1, TRect rt2);
	static bool  IntersectRect(TRect rt1, TRect rt2, TRect* rt);
	
	static bool   BoxToPoint(TBox rt, int x, int y, int z);
	static bool   BoxToPoint(TBox rt, TMath::TVector3 v);
	static TCollisionResult    BoxToBox(TBox, TBox);
	static TBox   UnionBox(TBox rt1, TBox rt2);
	static bool  IntersectBox(TBox rt1, TBox rt2, TBox* rt);
};

