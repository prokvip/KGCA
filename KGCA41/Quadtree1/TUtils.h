#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>

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
    TPoint() {}
    TPoint(float fx, float fy) : x(fx), y(fy)
    {
    }
};
struct TRect
{
    float m_fx;
    float m_fy;
    float m_fWidth;
    float m_fHeight;
    TPoint m_Point[4];
    TPoint m_Center;
    TPoint m_Half;
    TPoint m_Min;
    TPoint m_Max;
    TPoint v;
    TPoint s;
    /*TRect operator + (TRect& p)
    {
        return TRect(x + p.x, y + p.y);
    }
    TRect operator - (TPoint& p)
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

    void Set(float fx, float fy, float fw, float fh)
    {
        v = { fx, fy };
        s = { fw, fh };

        m_fx = fx;
        m_fy = fy;
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

    TRect() {}
    TRect(float fx, float fy, float fw, float fh)         
    {
        Set(fx, fy, fw, fh);
    }
};
class TUtils
{
};

