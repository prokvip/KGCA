#include "TBaseMath.h"
namespace T_MATH
{
    bool xVector2::operator == (xVector2& p)
    {
        if (fabs(x - p.x) > T_EPSILON)
        {
            if (fabs(y - p.y) > T_EPSILON)
            {
                return true;
            }
        }
        return false;
    }
    bool xVector2::operator != (xVector2& p)
    {
        return !(*this == p);
    }
    xVector2 xVector2::operator + (xVector2& p)
    {
        return xVector2(x + p.x, y + p.y);
    }
    xVector2 xVector2::operator - (xVector2& p)
    {
        return xVector2(x - p.x, y - p.y);
    }
    xVector2 xVector2::operator * (float fValue)
    {
        return xVector2(x * fValue, y * fValue);
    }
    xVector2 xVector2::operator / (float fValue)
    {
        return xVector2(x / fValue, y / fValue);
    }
    xVector2& xVector2::operator /= (float fValue)
    {
        x = x / fValue;
        y = y / fValue;
        return *this;
    }
    float xVector2::Length()
    {
        float fDistance = sqrt(x * x + y * y);
        return fDistance;
    }
    static float Length(xVector2& p)
    {
        float fDistance = sqrt(p.x * p.x + p.y * p.y);
        return fDistance;
    }
    xVector2::xVector2() {}
    xVector2::xVector2(float fx, float fy)
    {
        x = fx;
        y = fy;
    }
    float xVector3::operator | (xVector3 const& v)
    {
        return x * v.x + y * v.y + z * v.z;
    }
    xVector3 xVector3::operator ^ (xVector3 const& v)
    {
        return xVector3(y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }
    bool xVector3::operator == (xVector3& p)
    {
        if (fabs(x - p.x) > T_EPSILON)
        {
            if (fabs(y - p.y) > T_EPSILON)
            {
                if (fabs(z - p.z) > T_EPSILON)
                {
                    return true;
                }
            }
        }
        return false;
    }
    bool xVector3::operator != (xVector3& p)
    {
        return !(*this == p);
    }
    xVector3 xVector3::operator + (xVector3& p)
    {
        return xVector3(x + p.x, y + p.y, z + p.z);
    }
    xVector3 xVector3::operator - (xVector3& p)
    {
        return xVector3(x - p.x, y - p.y, z - p.z);
    }
    xVector3 xVector3::operator * (float fValue)
    {
        return xVector3(x * fValue, y * fValue, z * fValue);
    }
    xVector3 xVector3::operator / (float fValue)
    {
        return xVector3(x / fValue, y / fValue, z / fValue);
    }
    xVector3& xVector3::operator /= (float fValue)
    {
        x = x / fValue;
        y = y / fValue;
        z = z / fValue;
        return *this;
    }
    float xVector3::Length()
    {
        float fDistance = sqrt(x * x + y * y + z * z);
        return fDistance;
    }
    static float GetDistance(xVector3& p)
    {
        float fDistance = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
        return fDistance;
    }
    xVector3::xVector3() {}
    xVector3::xVector3(float fx, float fy, float fz)
    {
        x = fx;
        y = fy;
        z = fz;
    }
    xVector3 xVector3::operator * (xMatrix const& m)
    {
        xVector4 t;
        t.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
        t.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
        t.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
        t.w = x * m._14 + y * m._24 + z * m._34 + 1.0f * m._44;

        if (fabs(t.w - 0.0f) > T_EPSILON)
        {
            t.x /= t.w;
            t.y /= t.w;
            t.z /= t.w;
            t.w /= t.w;
        }
        return xVector3(t.x, t.y, t.z);
    }
    void xVector3::Normalize()
    {
        float fInvertLength = 1.0f / Length();
        x = x * fInvertLength;
        y = y * fInvertLength;
        z = z * fInvertLength;
    }
    xVector3 xVector3::NormalVector()
    {
        float fInvertLength = 1.0f / Length();
        return (*this) * fInvertLength;
    }
    float xVector3::Angle(xVector3& v)
    {
        // ¡§±‘»≠
        xVector3 a = NormalVector();
        xVector3 b = v.NormalVector();
        float fCosAngle = a | b;
        float fRadian = acos(fCosAngle);
        float fDegree = RadianToDegree(fRadian);
        return fDegree;
        /* float fLength1 = Length();
         float fLength2 = v.Length();
         float fDot = *this | v;
         float fCosAngle = fDot / (fLength1 * fLength2);
         float fRadian = acos(fCosAngle);
         float fDegree = RadianToDegree(fRadian);
         return fDegree;*/
    }

};