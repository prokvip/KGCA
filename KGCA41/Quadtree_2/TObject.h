#pragma once
#include "TCollision.h"

class TObject
{
public:
    TVector2D m_vDirection;
    TRect   m_rt;
    TCircle m_Circle;
    void   SetCircle(float x, float y, float w, float h)
    {
        m_Circle.cx = x;
        m_Circle.cy = y;
        float x1 = w / 2.0f;
        float y1 = h / 2.0f;
        m_Circle.fRadius = sqrt(x1 * x1 + y1 * y1);
    }
    void   SetPosition(float x, float y, float w, float h)
    {
        m_rt.Set(x, y, w, h);
        SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
    }
    void   Frame()
    {
        TVector2D vPos(m_rt.x1, m_rt.y1);
        // 벡터의 직전의 방정식
       //  결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간) 

        TVector2D vTmp = m_vDirection * 1.0f;
        vPos = vPos + vTmp;
        m_rt.x1 = vPos.x;
        m_rt.y1 = vPos.y;

        if (vPos.x > 100.0f)
        {
            m_rt.x1 = 100.0f;
            m_vDirection.x *= -1.0f;
        }
        if (vPos.x < 0.0f)
        {
            m_rt.x1 = 0.0f;
            m_vDirection.x *= -1.0f;
        }
        if (vPos.y > 100.0f)
        {
            m_rt.y1 = 100.0f;
            m_vDirection.y *= -1.0f;
        }
        if (vPos.y < 0.0f)
        {
            m_rt.y1 = 0.0f;
            m_vDirection.y *= -1.0f;
        }
        SetPosition(m_rt.x1, m_rt.y1, 30, 30);
    }
    TObject()
    {
        m_rt.Set(20 + (rand() % 80),
            20 + (rand() % 80),
            5.0f + (rand() % 5),
            5.0f + (rand() % 5));
        SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
        m_vDirection.x = cos(rand());
        m_vDirection.y = sin(rand());
        m_vDirection.Normalized();
    }
};
