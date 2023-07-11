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
        return TPoint(x + p.x , y + p.y);
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
    { }
};
class TNode
{
public:
    float m_fx; 
    float m_fy;
    float m_fWidth;
    float m_fHeight;
    TPoint m_Point[4];
    TPoint m_Center;
    TPoint m_Half;

    int    m_iValue = -1;
    int    m_iDepth = 0;
    TNode* m_pParent = nullptr;
    std::vector<TNode*> m_pChild;
    void   SetParent(TNode* pParent);
    TNode(TNode* pParent, 
        float x,
        float y, 
        float fWidth,
        float fHeight) : m_pParent(pParent)
    {
        m_fx = x;
        m_fy = y;
        m_fWidth = fWidth;
        m_fHeight = fHeight;
        m_Half = { m_fWidth * 0.5f,m_fHeight*0.5f };
        m_Point[0] = { m_fx, m_fy };
        m_Point[1] = { m_fx + m_fWidth, m_fy };
        m_Point[2] = { m_fx + m_fWidth, m_fy+ m_fHeight };
        m_Point[3] = { m_fx, m_fy + m_fHeight };
        m_Center = (m_Point[0] + m_Point[2]) * 0.5f;
        SetParent(pParent);
    }
};

