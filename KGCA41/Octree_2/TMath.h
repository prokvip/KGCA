#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <math.h>
#define T_PI (3.141592f)
#define T_EPSILON ( 0.0001f)
#define RadianToDegree(radian) ( radian *( 180.0f / T_PI))
#define DegreeToRadian(degree) ( degree *( T_PI / 180.0f))
#define randstep(fmin, fmax) ((float)fmin+((float)fmax-(float)fmin)* rand() / RAND_MAX)
struct TFloat2
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float v[2];
    };
};
struct TFloat3
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float v[3];
    };
};
class TVector2 : public TFloat2
{
public:
    bool operator == (TVector2& p);
    bool operator != (TVector2& p);
    TVector2 operator + (TVector2& p);
    TVector2 operator - (TVector2& p);
    TVector2 operator * (float fValue);
    TVector2 operator / (float fValue);
    TVector2& operator /= (float fValue);
public:
    float Length();
    static float Length(TVector2& p);
public:
    TVector2();
    TVector2(float fx, float fy);
};
class TVector3 : public TFloat3
{
public:
    float   operator | (TVector3 const& v);
    TVector3 operator ^ (TVector3 const& v);
    bool operator == (TVector3& p);
    bool operator != (TVector3& p);
    TVector3 operator + (TVector3& p);
    TVector3 operator - (TVector3& p);
    TVector3 operator * (float fValue);
    TVector3 operator / (float fValue);
    TVector3& operator /= (float fValue);
    float Angle(TVector3& v);
public:
    float Length();
    void Normalize();
    TVector3 NormalVector();   
public:
    static float GetDistance(TVector3& p);
public:
    TVector3();
    TVector3(float fx, float fy, float fz);
};

// 직선과 한점의 거리
class TRay
{
public:
    TVector3 vPosition;
    TVector3 vDirection;
    float GetDistance(TVector3& p0)
    {
        TVector3 v = p0 - vPosition;
        float k = ((v | vDirection) / (vDirection | vDirection));
        TVector3 p = vDirection * k;
        TVector3 w = v - p;
        return w.Length();
    }
    TRay() {};
    TRay(TVector3 p, TVector3 d)
    {
        vPosition = p;
        vDirection = d;
    }
};
// 평면과 한점의 거리
class TPlane
{
public:
    TVector3 vPos;
    TVector3 vNor;
    float a, b, c, d; 
    // nx*px+ny*py+nz+pz+d = 0;
    // -Dot(p, n) = d;
    float GetDistance(TVector3& v)
    {
        return a * v.x + b * v.y + c * v.z + d;
    }
    TPlane() {};
    TPlane(TVector3 vP, TVector3 vD) {
        vNor = vD.NormalVector();
        vPos = vP;
        a = vNor.x;
        b = vNor.y;
        c = vNor.z;
        d = -(vNor | vPos);        
    };
    // 시계방향으로 구성되어 있어야 한다.
    //   p0
    //p2    p1
    TPlane(TVector3 p0, TVector3 p1, TVector3 p2) 
    {
        vPos = p0;
        TVector3 e1 = p1 - p0;
        TVector3 e2 = p2 - p0;
        vNor = (e1 ^ e2).NormalVector();
        a = vNor.x;
        b = vNor.y;
        c = vNor.z;
        d = -(vNor | vPos);        
    };
};

class TMath
{
};

