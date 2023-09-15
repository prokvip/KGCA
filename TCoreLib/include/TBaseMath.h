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

namespace T_MATH
{
    class xVector2;
    class xVector3;
    class xVector4;
    class xMatrix;
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
    class xVector2 : public TFloat2
    {
    public:
        bool operator == (xVector2& p);
        bool operator != (xVector2& p);
        xVector2 operator + (xVector2& p);
        xVector2 operator - (xVector2& p);
        xVector2 operator * (float fValue);
        xVector2 operator / (float fValue);
        xVector2& operator /= (float fValue);
    public:
        float Length();
        static float Length(xVector2& p);
    public:
        xVector2();
        xVector2(float fx, float fy);
    };
    class xVector3 : public TFloat3
    {
    public:
        float   operator | (xVector3 const& v);
        xVector3 operator ^ (xVector3 const& v);
        bool operator == (xVector3& p);
        bool operator != (xVector3& p);
        xVector3 operator + (xVector3& p);
        xVector3 operator - (xVector3& p);
        xVector3 operator * (float fValue);
        xVector3 operator / (float fValue);
        xVector3& operator /= (float fValue);
        xVector3 operator * (xMatrix const& m);
        float Angle(xVector3& v);
    public:
        float Length();
        void Normalize();
        xVector3 NormalVector();
    public:
        static float GetDistance(xVector3& p);
    public:
        xVector3();
        xVector3(float fx, float fy, float fz);
    };
    class xVector4 : public TFloat4
    {
    public:
        xVector4()
        {
            x = y = z = w = 0.0f;
        }
        xVector4(float fx, float fy, float fz, float fw)
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
        xVector3 vPosition;
        xVector3 vDirection;
        float GetDistance(xVector3& p0)
        {
            xVector3 v = p0 - vPosition;
            float k = ((v | vDirection) / (vDirection | vDirection));
            xVector3 p = vDirection * k;
            xVector3 w = v - p;
            return w.Length();
        }
        TRay() {};
        TRay(xVector3 p, xVector3 d)
        {
            vPosition = p;
            vDirection = d;
        }
    };
    // 평면과 한점의 거리
    class TPlane
    {
    public:
        xVector3 vPos;
        xVector3 vNor;
        float a, b, c, d;
        // nx*px+ny*py+nz+pz+d = 0;
        // -Dot(p, n) = d;
        float GetDistance(xVector3& v)
        {
            return a * v.x + b * v.y + c * v.z + d;
        }
        TPlane() {};
        TPlane(xVector3 vP, xVector3 vD)
        {
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
        TPlane(xVector3 p0, xVector3 p1, xVector3 p2)
        {
            vPos = p0;
            xVector3 e1 = p1 - p0;
            xVector3 e2 = p2 - p0;
            vNor = (e1 ^ e2).NormalVector();
            a = vNor.x;
            b = vNor.y;
            c = vNor.z;
            d = -(vNor | vPos);
        };
    };

    class xMatrix : public TFloat4x4
    {
    public:
        xMatrix()
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
        xMatrix operator * (xMatrix const& matrix)
        {
            xMatrix ret;
            for (int iColumn = 0; iColumn < 4; iColumn++)
            {
                for (int iRow = 0; iRow < 4; iRow++)
                {
                    ret.m[iRow][iColumn] =
                        m[iRow][0] * matrix.m[0][iColumn] +
                        m[iRow][1] * matrix.m[1][iColumn] +
                        m[iRow][2] * matrix.m[2][iColumn] +
                        m[iRow][3] * matrix.m[3][iColumn];
                }
            }
            return ret;
        }
        xVector3 operator * (xVector3 const& v)
        {
            xVector4 ret;
            ret.w = 1.0f;
            ret.x = v.x * _11 + v.y * _21 + v.z * _31 + 1.0f * _41;
            ret.y = v.x * _12 + v.y * _22 + v.z * _32 + 1.0f * _42;
            ret.z = v.x * _13 + v.y * _23 + v.z * _33 + 1.0f * _43;
            ret.w = v.x * _14 + v.y * _24 + v.z * _34 + 1.0f * _44;

            /* ret.x /= ret.w;
             ret.y /= ret.w;
             ret.z /= ret.w;
             ret.w /= ret.w;*/

            return xVector3(ret.x, ret.y, ret.z);
        }
        float operator () (int iRow, int icol) const
        {
            return m[iRow][icol];
        }
        float& operator () (int iRow, int icol)
        {
            return m[iRow][icol];
        }

        void Translation(const xVector3& v)
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

        void Scale(const xVector3& v)
        {
            _11 = v.x;
            _22 = v.y;
            _33 = v.z;
        }
        void Scale(float x, float y, float z)
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
        xMatrix Transpose()
        {
            xMatrix ret;
            ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
            ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
            ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
            ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;
            return ret;
        }
    };
};

