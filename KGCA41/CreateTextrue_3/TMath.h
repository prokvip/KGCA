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

class TVector2;
class TVector3;
class TVector4;
class TMatrix;
class TRay;
class TPlane;

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
struct TFloat4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4];
    };
};
struct TFloat4x4
{
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
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
    TVector3 operator * (TMatrix const& m);
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
class TVector4 : public TFloat4
{
public:
    TVector4()
    {
        x = y = z = w = 0.0f;
    }
    TVector4(float fx, float fy, float fz, float fw)
    {
        x = fx;
        y = fy;
        z = fz;
        w = fw;
    }
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

class TMatrix : public TFloat4x4
{
public:
    TMatrix()
    {
        Identity();
    }
    void Identity()
    {
        _11 = _12 = _13 = _14 = 0.0f;
        _21 = _22 = _23 = _24 = 0.0f;
        _31 = _32 = _33 = _34 = 0.0f;
        _41 = _42 = _43 = _44 = 0.0f;
        _11 = _22 = _33 = _44 = 1.0f;
    }
    TMatrix operator * (TMatrix const& matrix)
    {
        TMatrix ret;
        for (int iColumn = 0; iColumn < 4; iColumn++)
        {
            for (int iRow = 0; iRow < 4; iRow++)
            {
                ret.m[iRow][iColumn] = 
                    m[iRow][0] * matrix.m[0][iColumn] +
                    m[iRow][1] * matrix.m[1][iColumn] +
                    m[iRow][2] * matrix.m[2][iColumn] +
                    m[iRow][3] * matrix.m[3][iColumn] ;
            }
        }
        return ret;
    }
    TVector3 operator * (TVector3 const& v)
    {
        TVector4 ret;
        ret.w = 1.0f;
        ret.x = v.x * _11 + v.y * _21 + v.z * _31 + 1.0f * _41;
        ret.y = v.x * _12 + v.y * _22 + v.z * _32 + 1.0f * _42;
        ret.z = v.x * _13 + v.y * _23 + v.z * _33 + 1.0f * _43;
        ret.w = v.x * _14 + v.y * _24 + v.z * _34 + 1.0f * _44;

       /* ret.x /= ret.w;
        ret.y /= ret.w;
        ret.z /= ret.w;
        ret.w /= ret.w;*/

        return TVector3(ret.x, ret.y, ret.z);
    }
    float operator () (int iRow, int icol) const
    {
        return m[iRow][icol];
    }
    float& operator () (int iRow, int icol)
    {
        return m[iRow][icol];
    }

    void Translation(const TVector3& v)
    {
        _41 = v.x;
        _42 = v.y;
        _43 = v.z;
    }
    void Translation(float x, float y, float z)
    {
        _41 = x;
        _42 = y;
        _43 = z;
    }

    void Sclae(const TVector3& v)
    {
        _11 = v.x;
        _22 = v.y;
        _33 = v.z;
    }
    void Sclae(float x, float y, float z)
    {
        _11 = x;
        _22 = y;
        _33 = z;
    }

    void XRotate(float fRadian)
    {
        float fCos = cos(fRadian);
        float fSin = sin(fRadian);
        _22 = fCos;  _23 = fSin;
        _32 = -fSin;  _33 = fCos;
    }
    void YRotate(float fRadian)
    {
        float fCos = cos(fRadian);
        float fSin = sin(fRadian);
        _11 = fCos;  _13 = -fSin;
        _31 = fSin; _33 = fCos;
    }
    void ZRotate(float fRadian)
    {
        float fCos = cos(fRadian);
        float fSin = sin(fRadian);
        _11 = fCos;  _12 = fSin;
        _21 = -fSin; _22 = fCos;
    }
    TMatrix Transpose()
    {
        TMatrix ret;
        ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
        ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
        ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
        ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;
        return ret;
    }
};
class TMath
{
};

