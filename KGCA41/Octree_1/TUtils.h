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
    float x = 0.0f;
    float y = 0.0f;
    bool operator == (TPoint& p)
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
    bool operator != (TPoint& p)
    {
        return !(*this == p);
    }
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
    bool  m_bEnable = true;
    float m_fWidth = 0.0f;
    float m_fHeight = 0.0f;
    TPoint m_Point[4];
    TPoint m_Center;
    TPoint m_Half;
    TPoint m_Min;
    TPoint m_Max;
    TPoint v;
    TPoint s;
    bool Intersect(TRect& p, TRect& ret)
    {
        return false;
    }
    bool operator == (TRect& p)
    {
        if (fabs(m_fx - p.m_fx) > 0.0001f)
        {
            if (fabs(m_fy - p.m_fy) > 0.0001f)
            {
                if (fabs(m_fWidth - p.m_fWidth) > 0.0001f)
                {
                    if (fabs(m_fHeight - p.m_fHeight) > 0.0001f)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool operator != (TRect& p)
    {        
        return !(*this == p);        
    }
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
    TRect operator - (TRect& p)
    {
        TRect rt;
        rt.m_bEnable = false;
        if (ToRect(p))
        {
            //left, top          right
            //      bottom
            float fx = (m_Min.x > p.m_Min.x) ? m_Min.x : p.m_Min.x;
            float fy = (m_Min.y > p.m_Min.y) ? m_Min.y : p.m_Min.y;
            float right= (m_Max.x < p.m_Max.x) ? m_Max.x : p.m_Max.x;
            float bottom = (m_Max.y < p.m_Max.y) ? m_Max.y : p.m_Max.y;
            rt.Set(fx, fy, right-fx, bottom-fy);
            rt.m_bEnable = true;
        }
        return rt;
    }
    TRect operator - (TPoint& p)
    {
        m_fx -= p.x;
        m_fy -= p.y;
        return TRect(m_fx, m_fy, m_fWidth, m_fHeight);
    }
    TRect operator * (float fValue)
    {   
        if (fValue <= 0.0f)
        {            
            return *this;
        }
        m_fWidth *= fValue;
        m_fHeight *= fValue;   
        return TRect(m_fx, m_fy, m_fWidth, m_fHeight);
    }
    TRect operator / (float fValue)
    {
        if (fValue <= 0.0f)
        {
            return *this;
        }
        m_fWidth /= fValue;
        m_fHeight /= fValue;
        return TRect(m_fx, m_fy, m_fWidth, m_fHeight);
    }
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

    bool ToRect(TRect& rt)
    {
        TRect sum = (*this) + rt;
        float fX = m_fWidth + rt.m_fWidth;
        float fY = m_fHeight + rt.m_fHeight;
        if (sum.m_fWidth <= fX)
        {
            if (sum.m_fHeight <= fY)
            {
                return true;
            }
        }
        return false;
    }
    bool ToPoint(TPoint& p)
    {
        if (m_Min.x <= p.x && m_Max.x >= p.x   &&
            m_Min.y <= p.y && m_Max.y >= p.y)
        {
            return true;
        }
        return false;
    }
    TRect() : m_bEnable(true) {}

    TRect(float fx, float fy, float fw, float fh)         
    {
        m_bEnable = true;
        Set(fx, fy, fw, fh);
    }
};
class TUtils
{
};

