#include "TMath.h"
bool TVector2::operator == (TVector2& p)
{
    if (fabs(x - p.x) > 0.0001f)
    {
        if (fabs(y - p.y) > 0.0001f)
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
float TVector2::GetDistance()
{
    float fDistance = sqrt(x * x + y * y);
    return fDistance;
}
static float GetDistance(TVector2& p)
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
    if (fabs(x - p.x) > 0.0001f)
    {
        if (fabs(y - p.y) > 0.0001f)
        {
            if (fabs(z - p.z) > 0.0001f)
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
float TVector3::GetDistance()
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