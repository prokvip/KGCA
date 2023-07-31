#pragma once
#include "TMath.h"
struct TRect 
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;    
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
    bool Intersect(TRect& p, TRect& ret)
    {
        return false;
    }
    bool operator == (TRect& p)
    {
        if (fabs(x - p.x) > 0.0001f)
        {
            if (fabs(y - p.y) > 0.0001f)
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
        float fMinX = min(x, p.x);
        float fMinY = min(y, p.y);
        float fMaxX = max(m_Point[2].x, p.m_Point[2].x);
        float fMaxY = max(m_Point[2].y, p.m_Point[2].y);
        TVector2 pos = { fMinX, fMinY };
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
    TRect operator - (TVector2& p)
    {
        x -= p.x;
        y -= p.y;
        return TRect(x, y, m_fWidth, m_fHeight);
    }
    TRect operator * (float fValue)
    {   
        if (fValue <= 0.0f)
        {            
            return *this;
        }
        m_fWidth *= fValue;
        m_fHeight *= fValue;   
        return TRect(x, y, m_fWidth, m_fHeight);
    }
    TRect operator / (float fValue)
    {
        if (fValue <= 0.0f)
        {
            return *this;
        }
        m_fWidth /= fValue;
        m_fHeight /= fValue;
        return TRect(x, y, m_fWidth, m_fHeight);
    }
    void Set(TVector2 p)
    {
        v = { p.x, p.y };
        s = { m_fWidth, m_fHeight };
        x = x;
        y = y;
        Set(m_fWidth, m_fHeight);
    }
    void Set(float fw, float fh)
    {
        m_fWidth = fw;
        m_fHeight = fh;
        m_Half = { m_fWidth * 0.5f,m_fHeight * 0.5f };
        m_Point[0] = { x, y };
        m_Point[1] = { x + m_fWidth, y };
        m_Point[2] = { x + m_fWidth, y + m_fHeight };
        m_Point[3] = { x, y + m_fHeight };
        m_Center = (m_Point[0] + m_Point[2]) * 0.5f;
        m_Min = m_Point[0];
        m_Max = m_Point[2];
    }
    void Set(TVector2 p, float fw, float fh)
    {
        v = { p.x, p.y };
        s = { fw, fh };
        x = p.x;
        y = p.y;
        Set(fw, fh);
    }
    void Set(float fx, float fy, float fw, float fh)
    {
        v = { fx, fy };
        s = { fw, fh };

        x = fx;
        y = fy;
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
    bool ToPoint(TVector2& p)
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
struct TBox 
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
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

    bool operator == (TBox& p)
    {
        if (fabs(x - p.x) > 0.0001f)
        {
            if (fabs(y - p.y) > 0.0001f)
            {
                if (fabs(z - p.z) > 0.0001f)
                {
                    if (fabs(m_fWidth - p.m_fWidth) > 0.0001f)
                    {
                        if (fabs(m_fHeight - p.m_fHeight) > 0.0001f)
                        {
                            if(fabs(m_fDepth - p.m_fDepth) > 0.0001f)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    bool operator != (TBox& p)
    {
        return !(*this == p);
    }
    // union
    TBox operator + (TBox& p)
    {
        TBox rt;
        float fMinX = min(x, p.x);
        float fMinY = min(y, p.y);
        float fMinZ = min(z, p.z);
        float fMaxX = max(m_Max.x, p.m_Max.x);
        float fMaxY = max(m_Max.y, p.m_Max.y);
        float fMaxZ = max(m_Max.z, p.m_Max.z);
        TVector3 pos = { fMinX, fMinY, fMinZ };
        rt.Set(pos, fMaxX - fMinX, fMaxY - fMinY, fMaxZ - fMinZ);
        return rt;
    }
    TBox operator - (TBox& p)
    {
        TBox rt;
        rt.m_bEnable = false;
        if (ToBox(p))
        {
            //left, top          right
            //      bottom
            float fx = (m_Min.x > p.m_Min.x) ? m_Min.x : p.m_Min.x;
            float fy = (m_Min.y > p.m_Min.y) ? m_Min.y : p.m_Min.y;
            float fz = (m_Min.z > p.m_Min.z) ? m_Min.z : p.m_Min.z;

            float right = (m_Max.x < p.m_Max.x) ? m_Max.x : p.m_Max.x;
            float bottom = (m_Max.y < p.m_Max.y) ? m_Max.y : p.m_Max.y;
            float depth = (m_Max.z < p.m_Max.z) ? m_Max.z : p.m_Max.z;
            rt.Set(fx, fy, fz,  right - fx, bottom - fy, depth-fz);
            rt.m_bEnable = true;
        }
        return rt;
    }
    TBox operator - (TVector3& p)
    {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return TBox(x, y, z,  m_fWidth, m_fHeight, m_fDepth);
    }
    TBox operator * (float fValue)
    {
        if (fValue <= 0.0f)
        {
            return *this;
        }
        m_fWidth *= fValue;
        m_fHeight *= fValue;
        m_fDepth *= fValue;
        return TBox(x, y, z,  m_fWidth, m_fHeight, m_fDepth);
    }
    TBox operator / (float fValue)
    {
        if (fValue <= 0.0f)
        {
            return *this;
        }
        m_fWidth /= fValue;
        m_fHeight /= fValue;
        m_fDepth /= fValue;
        return TBox(x, y, z,  m_fWidth, m_fHeight, m_fDepth);
    }
    void Set(TVector3 p)
    {
        v = { p.x, p.y, p.z };
        s = { m_fWidth, m_fHeight, m_fDepth };
        x = x;
        y = y;
        z = z;
        Set(m_fWidth, m_fHeight, m_fDepth);
    }
    void Set(float fSizeX, float fSizeY, float fSizeZ)
    {
        m_fWidth = fSizeX;
        m_fHeight = fSizeY;
        m_fDepth = fSizeZ;
        m_Half = { m_fWidth * 0.5f,m_fHeight * 0.5f, m_fDepth * 0.5f };
        m_Point[0] = { x, y, z };
        m_Point[1] = { x + m_fWidth, y, z };
        m_Point[2] = { x + m_fWidth, y + m_fHeight, z };
        m_Point[3] = { x, y + m_fHeight, z };

        m_Point[4] = { x, y, z + fSizeZ };
        m_Point[5] = { x + m_fWidth, y, z + fSizeZ };
        m_Point[6] = { x + m_fWidth, y + m_fHeight, z + fSizeZ };
        m_Point[7] = { x, y + m_fHeight, z + fSizeZ };

        m_Min = m_Point[0];
        m_Max = m_Point[6];
        m_Center = (m_Max + m_Min) * 0.5f;       
    }
    void Set(TVector3 p, float fw, float fh, float fz)
    {
        v = { p.x, p.y, p.z };       
        s = { fw, fh, fz };
        x = p.x;
        y = p.y;
        z = p.z;
        Set(fw, fh, fz);
    }
    void Set(float fx, float fy, float fz,  float fw, float fh, float fd)
    {
        v = { fx, fy, fz };
        s = { fw, fh, fd };

        x = fx;
        y = fy;
        z = fz;
        Set(fw, fh, fd);
    }

    bool ToBox(TBox& rt)
    {
        TBox sum = (*this) + rt;
        float fX = m_fWidth + rt.m_fWidth;
        float fY = m_fHeight + rt.m_fHeight;
        float fZ = m_fDepth + rt.m_fDepth;
        if (sum.m_fWidth <= fX)
        {
            if (sum.m_fHeight <= fY)
            {
                if (sum.m_fDepth <= fZ)
                {
                    return true;
                }
            }
        }
        return false;
    }
    bool ToPoint(TVector3& p)
    {
        if (m_Min.x <= p.x && m_Max.x >= p.x &&
            m_Min.y <= p.y && m_Max.y >= p.y &&
            m_Min.z <= p.z && m_Max.z >= p.z)
        {
            return true;
        }
        return false;
    }
    TBox() : m_bEnable(true) {}

    TBox(float fx, float fy, float fz, 
         float fw, float fh, float fd)
    {
        m_bEnable = true;
        Set(fx, fy, fz, fw, fh, fz);
    }
};
class TUtils
{
};

