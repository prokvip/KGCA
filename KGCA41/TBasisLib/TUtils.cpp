#include "TUtils.h"

bool TRect::operator == (TRect& p)
{
    if (fabs(v.x - p.v.x) > 0.0001f)
    {
        if (fabs(v.y - p.v.y) > 0.0001f)
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
bool TRect::operator != (TRect& p)
{
    return !(*this == p);
}
TRect TRect::operator + (TRect& p)
{
    TRect rt;
    float fMinX = min(m_Min.x, p.m_Min.x);
    float fMinY = min(m_Min.y, p.m_Min.y);
    float fMaxX = max(m_Max.x, p.m_Max.x);
    float fMaxY = max(m_Max.y, p.m_Max.y);
    TVector2 center = { (fMinX+ fMaxX)*0.5f, (fMinY+ fMaxY)*0.5f };
    rt.Set(center, fMaxX - fMinX, fMaxY - fMinY);
    return rt;
}
TRect TRect::operator - (TRect& p)
{
    TRect rt;
    rt.m_bEnable = false;
    if (ToRect(p))
    {
        //left, top          right
        //      bottom
        float fx = (m_Min.x > p.m_Min.x) ? m_Min.x : p.m_Min.x;
        float fy = (m_Min.y > p.m_Min.y) ? m_Min.y : p.m_Min.y;
        float right = (m_Max.x < p.m_Max.x) ? m_Max.x : p.m_Max.x;
        float bottom = (m_Max.y < p.m_Max.y) ? m_Max.y : p.m_Max.y;
        TVector2 center = { (fx + right) * 0.5f, (fy + bottom) * 0.5f };
        rt.Set(center, right - fx, bottom - fy);
        rt.m_bEnable = true;
    }
    return rt;
}
TRect TRect::operator - (TVector2& p)
{
    v.x -= p.x;
    v.y -= p.y;
    return TRect(v.x, v.y, m_fWidth, m_fHeight);
}
TRect TRect::operator * (float fValue)
{
    if (fValue <= 0.0f)
    {
        return *this;
    }
    m_fWidth *= fValue;
    m_fHeight *= fValue;
    return TRect(v.x, v.y, m_fWidth, m_fHeight);
}
TRect TRect::operator / (float fValue)
{
    if (fValue <= 0.0f)
    {
        return *this;
    }
    m_fWidth /= fValue;
    m_fHeight /= fValue;
    return TRect(v.x, v.y, m_fWidth, m_fHeight);
}
void TRect::Set(TVector2 p)
{
    m_Center = p;
    v = { p.x, p.y };
    s = { m_fWidth, m_fHeight };
    Set(m_fWidth, m_fHeight);
}
void TRect::Set(float fw, float fh)
{
    m_fWidth = fw;
    m_fHeight = fh;
    m_Half = { m_fWidth * 0.5f,m_fHeight * 0.5f };
    m_Point[0] = { v.x, v.y };
    m_Point[1] = { v.x + m_fWidth, v.y };
    m_Point[2] = { v.x + m_fWidth, v.y - m_fHeight };
    m_Point[3] = { v.x, v.y - m_fHeight };    
    m_Max = m_Point[1];
    m_Min = m_Point[3];
}
void TRect::Set(TVector2 p, float fw, float fh)
{
    m_Center = p;
    v = { p.x - fw / 2.0f, p.y + fh / 2.0f };
    s = { fw, fh };
    Set(fw, fh);
}
void TRect::Set(float fx, float fy, float fw, float fh)
{
    m_Center.x = fx;
    m_Center.y = fy;
    s = { fw, fh };
    v = { fx-fw/2.0f, fy+ fh/2.0f };
    
    Set(fw, fh);
}

bool TRect::ToRect(TRect& rt)
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
bool TRect::ToPoint(TVector2& p)
{
    if (m_Min.x <= p.x && m_Max.x >= p.x &&
        m_Min.y <= p.y && m_Max.y >= p.y)
    {
        return true;
    }
    return false;
}
TRect::TRect() : m_bEnable(true) {}

TRect::TRect(float fx, float fy, float fw, float fh)
{
    m_bEnable = true;
    Set(fx, fy, fw, fh);
}


///
/////
bool TBox::operator == (TBox& p)
{
    if (fabs(v.x - p.v.x) > 0.0001f)
    {
        if (fabs(v.y - p.v.y) > 0.0001f)
        {
            if (fabs(v.z - p.v.z) > 0.0001f)
            {
                if (fabs(m_fWidth - p.m_fWidth) > 0.0001f)
                {
                    if (fabs(m_fHeight - p.m_fHeight) > 0.0001f)
                    {
                        if (fabs(m_fDepth - p.m_fDepth) > 0.0001f)
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
bool TBox::operator != (TBox& p)
{
    return !(*this == p);
}
// union
TBox TBox::operator + (TBox& p)
{
    TBox rt;
    float fMinX = min(v.x, p.v.x);
    float fMinY = min(v.y, p.v.y);
    float fMinZ = min(v.z, p.v.z);
    float fMaxX = max(m_Max.x, p.m_Max.x);
    float fMaxY = max(m_Max.y, p.m_Max.y);
    float fMaxZ = max(m_Max.z, p.m_Max.z);
    TVector3 pos = { fMinX, fMinY, fMinZ };
    rt.Set(pos, fMaxX - fMinX, fMaxY - fMinY, fMaxZ - fMinZ);
    return rt;
}
TBox TBox::operator - (TBox& p)
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
        rt.Set(fx, fy, fz, right - fx, bottom - fy, depth - fz);
        rt.m_bEnable = true;
    }
    return rt;
}
TBox TBox::operator - (TVector3& p)
{
    v.x -= p.x;
    v.y -= p.y;
    v.z -= p.z;
    return TBox(v.x, v.y, v.z, m_fWidth, m_fHeight, m_fDepth);
}
TBox TBox::operator * (float fValue)
{
    if (fValue <= 0.0f)
    {
        return *this;
    }
    m_fWidth *= fValue;
    m_fHeight *= fValue;
    m_fDepth *= fValue;
    return TBox(v.x, v.y, v.z, m_fWidth, m_fHeight, m_fDepth);
}
TBox TBox::operator / (float fValue)
{
    if (fValue <= 0.0f)
    {
        return *this;
    }
    m_fWidth /= fValue;
    m_fHeight /= fValue;
    m_fDepth /= fValue;
    return TBox(v.x, v.y, v.z, m_fWidth, m_fHeight, m_fDepth);
}
void TBox::Set(TVector3 p)
{
    v = p;
    s = { m_fWidth, m_fHeight, m_fDepth };
    Set(m_fWidth, m_fHeight, m_fDepth);
}
void TBox::Set(float fSizeX, float fSizeY, float fSizeZ)
{
    m_fWidth = fSizeX;
    m_fHeight = fSizeY;
    m_fDepth = fSizeZ;
    m_Half = { m_fWidth * 0.5f,m_fHeight * 0.5f, m_fDepth * 0.5f };
    m_Point[0] = { v.x - m_Half.x, v.y + m_Half.y, v.z - m_Half.z };
    m_Point[1] = m_Point[0];
    m_Point[1].x += m_fWidth;
    m_Point[2] = m_Point[1];
    m_Point[2].y -= m_fHeight;
    m_Point[3] = m_Point[2];
    m_Point[3].x = m_Point[0].x;

    m_Point[4] = { v.x - m_Half.x, v.y + m_Half.y, v.z + m_Half.z };
    m_Point[5] = m_Point[4];
    m_Point[5].x += m_fWidth;
    m_Point[6] = m_Point[5];
    m_Point[6].y -= m_fHeight;
    m_Point[7] = m_Point[6];
    m_Point[7].x = m_Point[4].x;

    m_Min = m_Point[3];
    m_Max = m_Point[5];
    m_Center = (m_Max + m_Min) * 0.5f;
}
void TBox::Set(TVector3 p, float fw, float fh, float fz)
{
    v = p;
    s = { fw, fh, fz };
    Set(fw, fh, fz);
}
void TBox::Set(float fx, float fy, float fz, float fw, float fh, float fd)
{
    v = { fx, fy, fz };
    s = { fw, fh, fd };
    Set(fw, fh, fd);
}

bool TBox::ToBox(TBox& rt)
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
bool TBox::ToPoint(TVector3& p)
{
    if (m_Min.x <= p.x && m_Max.x >= p.x &&
        m_Min.y <= p.y && m_Max.y >= p.y &&
        m_Min.z <= p.z && m_Max.z >= p.z)
    {
        return true;
    }
    return false;
}
TBox::TBox() : m_bEnable(true) {}

TBox::TBox(float fx, float fy, float fz,
    float fw, float fh, float fd)
{
    m_bEnable = true;
    Set(fx, fy, fz, fw, fh, fz);
}
