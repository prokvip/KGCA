#pragma once
#include "TMath.h"
struct TRect 
{
    bool  m_bEnable = true;
    float m_fWidth = 0.0f;
    float m_fHeight = 0.0f;
    TVector2 m_Point[4];
    TVector2 m_Center;
    TVector2 m_Half;
    TVector2 m_Min;
    TVector2 m_Max;
    TVector2 v;
    TVector2 s;
    bool Intersect(TRect& p, TRect& ret);
    bool operator == (TRect& p);
    bool operator != (TRect& p);
    TRect operator + (TRect& p);
    TRect operator - (TRect& p);
    TRect operator - (TVector2& p);
    TRect operator * (float fValue);
    TRect operator / (float fValue);
    void Set(TVector2 p);
    void Set(float fw, float fh);
    void Set(TVector2 p, float fw, float fh);
    void Set(float fx, float fy, float fw, float fh);

    bool ToRect(TRect& rt);
    bool ToPoint(TVector2& p);
    TRect();
    TRect(float fx, float fy, float fw, float fh);
};
struct TBox 
{
    bool  m_bEnable = true;
    float m_fWidth = 0.0f;
    float m_fHeight = 0.0f;
    float m_fDepth = 0.0f;
    TVector3 m_Point[8];
    TVector3 m_Center;
    TVector3 m_Half;
    TVector3 m_Min;
    TVector3 m_Max;
    TVector3 v;
    TVector3 s;

    bool operator == (TBox& p);
    bool operator != (TBox& p);
    // union
    TBox operator + (TBox& p);
    TBox operator - (TBox& p);
    TBox operator - (TVector3& p);
    TBox operator * (float fValue);
    TBox operator / (float fValue);
    void Set(TVector3 p);
    void Set(float fSizeX, float fSizeY, float fSizeZ);
    void Set(TVector3 p, float fw, float fh, float fz);
    void Set(float fx, float fy, float fz, float fw, float fh, float fd);

    bool ToBox(TBox& rt);
    bool ToPoint(TVector3& p);
    TBox();

    TBox(float fx, float fy, float fz,
        float fw, float fh, float fd);
};
class TUtils
{
};

