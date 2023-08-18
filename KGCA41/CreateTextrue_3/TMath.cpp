#include "TMath.h"
bool TVector2::operator == (TVector2& p)
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
bool TVector2::operator != (TVector2& p)
{
    return !(*this == p);
}
TVector2 TVector2::operator + (TVector2& p)
{
    return TVector2(x + p.x, y + p.y);
}
TVector2 TVector2::operator - (TVector2& p)
{
    return TVector2(x - p.x, y - p.y);
}
TVector2 TVector2::operator * (float fValue)
{
    return TVector2(x * fValue, y * fValue);
}
TVector2 TVector2::operator / (float fValue)
{
    return TVector2(x / fValue, y / fValue);
}
TVector2& TVector2::operator /= (float fValue)
{
    x = x / fValue;
    y = y / fValue;
    return *this;
}
float TVector2::Length()
{
    float fDistance = sqrt(x * x + y * y);
    return fDistance;
}
static float Length(TVector2& p)
{
    float fDistance = sqrt(p.x * p.x + p.y * p.y);
    return fDistance;
}
TVector2::TVector2() {}
TVector2::TVector2(float fx, float fy)
{
    x = fx;
    y = fy;
}
float TVector3::operator | (TVector3 const& v)
{
    return x * v.x + y * v.y + z * v.z;
}
TVector3 TVector3::operator ^ (TVector3 const& v)
{
    return TVector3( y * v.z - z*v.y,
                    z * v.x - x * v.z,
                    x * v.y - y * v.x);
}
bool TVector3::operator == (TVector3& p)
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
bool TVector3::operator != (TVector3& p)
{
    return !(*this == p);
}
TVector3 TVector3::operator + (TVector3& p)
{
    return TVector3(x + p.x, y + p.y, z + p.z);
}
TVector3 TVector3::operator - (TVector3& p)
{
    return TVector3(x - p.x, y - p.y, z - p.z);
}
TVector3 TVector3::operator * (float fValue)
{
    return TVector3(x * fValue, y * fValue, z * fValue);
}
TVector3 TVector3::operator / (float fValue)
{
    return TVector3(x / fValue, y / fValue, z / fValue);
}
TVector3& TVector3::operator /= (float fValue)
{
    x = x / fValue;
    y = y / fValue;
    z = z / fValue;
    return *this;
}
float TVector3::Length()
{
    float fDistance = sqrt(x * x + y * y + z * z);
    return fDistance;
}
static float GetDistance(TVector3& p)
{
    float fDistance = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    return fDistance;
}
TVector3::TVector3() {}
TVector3::TVector3(float fx, float fy, float fz) 
{
    x = fx;
    y = fy;
    z = fz;
}
TVector3 TVector3::operator * (TMatrix const& m)
{
    TVector4 t;
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
    return TVector3(t.x, t.y, t.z);
}
void TVector3::Normalize()
{
    float fInvertLength = 1.0f / Length();
    x = x * fInvertLength;
    y = y * fInvertLength;
    z = z * fInvertLength;
}
TVector3 TVector3::NormalVector()
{
    float fInvertLength = 1.0f / Length();    
    return (*this) * fInvertLength;
}
float TVector3::Angle(TVector3& v)
{
    // ¡§±‘»≠
    TVector3 a = NormalVector();
    TVector3 b = v.NormalVector();
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