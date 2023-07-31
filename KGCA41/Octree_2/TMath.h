#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <math.h>

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
struct TVector2 : TFloat2
{
    bool operator == (TVector2& p);
    bool operator != (TVector2& p);
    TVector2 operator + (TVector2& p);
    TVector2 operator - (TVector2& p);
    TVector2 operator * (float fValue);
    TVector2 operator / (float fValue);
    TVector2& operator /= (float fValue);
    float GetDistance();
    static float GetDistance(TVector2& p);
    TVector2();
    TVector2(float fx, float fy);
};
struct TVector3 : TFloat3
{
    float   operator | (TVector3 const& v);
    TVector3 operator ^ (TVector3 const& v);
    bool operator == (TVector3& p);
    bool operator != (TVector3& p);
    TVector3 operator + (TVector3& p);
    TVector3 operator - (TVector3& p);
    TVector3 operator * (float fValue);
    TVector3 operator / (float fValue);
    TVector3& operator /= (float fValue);
    float GetDistance();
    static float GetDistance(TVector3& p);
    TVector3();
    TVector3(float fx, float fy, float fz);
};


class TMath
{
};

