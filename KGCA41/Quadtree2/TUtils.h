#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <math.h>

#define randstep(fmin, fmax) ((float)fmin+((float)fmax-(float)fmin)* rand() / RAND_MAX)
struct TPoint
{
    float x;
    float y;
    TPoint operator + (TPoint& p)
    {
        return TPoint(x + p.x, y + p.y);
    }
    TPoint operator - (TPoint& p)
    {
        return TPoint(x - p.x, y - p.y);
    }
    TPoint operator * (float fValue)
    {
        return TPoint(x * fValue, y * fValue);
    }
    TPoint operator / (float fValue)
    {
        return TPoint(x / fValue, y / fValue);
    }
    TPoint& operator /= (float fValue)
    {
        x = x / fValue;
        y = y / fValue;
        return *this;
    }
    float GetDistance()
    {
        float fDistance = sqrt(x * x + y * y);
        return fDistance;
    }
    static float GetDistance(TPoint& p)
    {
        float fDistance = sqrt(p.x * p.x + p.y * p.y);
        return fDistance;
    }
    TPoint() {}
    TPoint(float fx, float fy) : x(fx), y(fy)
    {
    }
};
struct TFloat2
{
    union
    {
        struct
        {
            float m_fx;
            float m_fy;
        };
        float v[2];        
    };
};
struct TRect : TFloat2
{
    float m_fWidth;
    float m_fHeight;
    TPoint m_Point[4];
    TPoint m_Center;
    TPoint m_Half;
    TPoint m_Min;
    TPoint m_Max;
    TPoint v;
    TPoint s;
    TRect operator + (TRect& p)
    {
        TRect rt;
        float fMinX = min(m_fx, p.m_fx);
        float fMinY = min(m_fy, p.m_fy);
        float fMaxX = max(m_Point[2].x, p.m_Point[2].x);
        float fMaxY = max(m_Point[2].y, p.m_Point[2].y);
        TPoint pos = { fMinX, fMinY };
        rt.Set(pos, fMaxX- fMinX, fMaxY - fMinY);
        return rt;
    }
    /*TRect operator - (TPoint& p)
    {
        return TRect(x - p.x, y - p.y);
    }
    TRect operator * (float fValue)
    {
        return TRect(x * fValue, y * fValue);
    }
    TRect operator / (float fValue)
    {
        return TRect(x / fValue, y / fValue);
    }*/
    void Set(TPoint p)
    {
        v = { p.x, p.y };
        s = { m_fWidth, m_fHeight };
        m_fx = m_fx;
        m_fy = m_fy;
        Set(m_fWidth, m_fHeight);
    }
    void Set(float fw, float fh)
    {
        m_fWidth = fw;
        m_fHeight = fh;
        m_Half = { m_fWidth * 0.5f,m_fHeight * 0.5f };
        m_Point[0] = { m_fx, m_fy };
        m_Point[1] = { m_fx + m_fWidth, m_fy };
        m_Point[2] = { m_fx + m_fWidth, m_fy + m_fHeight };
        m_Point[3] = { m_fx, m_fy + m_fHeight };
        m_Center = (m_Point[0] + m_Point[2]) * 0.5f;
        m_Min = m_Point[0];
        m_Max = m_Point[2];
    }
    void Set(TPoint p, float fw, float fh)
    {
        v = { p.x, p.y };
        s = { fw, fh };
        m_fx = p.x;
        m_fy = p.y;
        Set(fw, fh);
    }
    void Set(float fx, float fy, float fw, float fh)
    {
        v = { fx, fy };
        s = { fw, fh };

        m_fx = fx;
        m_fy = fy;
        Set(fw, fh);
    }

    TRect() {}

    TRect(float fx, float fy, float fw, float fh)         
    {
        Set(fx, fy, fw, fh);
    }
};
class TUtils
{
};

