#pragma once
#include "TBasisFBXStd.h"
namespace TBasisFBX
{
	enum TCollisionResult
	{
		RECT_OUT = 0,
		RECT_IN,
		RECT_OVERLAP,
	};
	// ȭ����ǥ��+���ʻ���� ����
	struct TRect2D
	{
		TVector2 vMin;
		TVector2 vMax;
		TVector2 vCenter;
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
		TRect2D(TBasisFBX::TVector2 vMin, TBasisFBX::TVector2 vMax)
		{
			this->vMin = vMin;
			this->vMax = vMax;
			vCenter = (vMax + vMin);
			vCenter /= 2.0f;
			size.x = vMax.x - vMin.x;
			size.y = vMax.y - vMin.y;
		}
		TRect2D(TBasisFBX::TVector2 v, float w, float h)
		{
			this->vMin = v;
			this->vMax = vMin + TBasisFBX::TVector2(w, h);
			vCenter = (vMax + vMin);
			vCenter /= 2.0f;
			this->size.x = w;
			this->size.y = h;
		}
	};
	// ȭ����ǥ��+�߾��� ����
	struct TRect
	{
		TVector2 vMin;
		TVector2 vMax;
		TVector2 vCenter;
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
		TRect(TBasisFBX::TVector2 vMin, TBasisFBX::TVector2 vMax)
		{
			this->vMin = vMin;
			this->vMax = vMax;
			vCenter = (vMax + vMin);
			vCenter /= 2.0f;
			size.x = vMax.x - vMin.x;
			size.y = vMax.y - vMin.y;
		}
		TRect(TBasisFBX::TVector2 pos, float w, float h)
		{
			vCenter = pos;
			this->vMin = vCenter - TBasisFBX::TVector2(w / 2.0f, h / 2.0f);
			this->vMax = vCenter + TBasisFBX::TVector2(w / 2.0f, h / 2.0f);
			this->size.x = w;
			this->size.y = h;
		}
	};
	struct TBox
	{
		TVector3 vList[8];
		TVector3 vMin;
		TVector3 vMax;
		TVector3 vCenter;
		TVector3 vAxis[3];
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
		TBox(TBasisFBX::TVector3 vMin, TBasisFBX::TVector3 vMax)
		{
			this->vMin = vMin;
			this->vMax = vMax;
			vCenter = (vMax + vMin);
			vCenter /= 2.0f;
			size.x = vMax.x - vMin.x;
			size.y = vMax.y - vMin.y;
		}
		TBox(TBasisFBX::TVector3 v, float w, float h, float q)
		{
			this->vMin = v;
			this->vMax = vMin + TBasisFBX::TVector3(w, h, q);
			vCenter = (vMax + vMin);
			vCenter /= 2.0f;
			this->size.x = w;
			this->size.y = h;
		}
	};
	struct TSphere
	{
		TBasisFBX::TVector3 vCenter;
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
		static bool   SphereToPoint(TSphere rt, TBasisFBX::TVector3 v);
		static bool   RectToPoint(TRect rt, int x, int y);
		static bool   RectToPoint(TRect rt, TBasisFBX::TVector2 v);
		static TCollisionResult    RectToRect(TRect, TRect);
		static TCollisionResult    ToRect(TRect rt1, TRect rt2);
		static TRect  UnionRect(TRect rt1, TRect rt2);
		static bool  IntersectRect(TRect rt1, TRect rt2, TRect* rt);

		static bool   BoxToPoint(TBox rt, int x, int y, int z);
		static bool   BoxToPoint(TBox rt, TBasisFBX::TVector3 v);
		static TCollisionResult    BoxToBox(TBox, TBox);
		static TBox   UnionBox(TBox rt1, TBox rt2);
		static bool  IntersectBox(TBox rt1, TBox rt2, TBox* rt);
	};

};