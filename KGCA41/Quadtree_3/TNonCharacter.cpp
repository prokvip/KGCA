#include "TNonCharacter.h"

// 뉴턴의 운동법칙
// 1)관성의 법칙:상태를 유지 할려는 특성
// 2)가속도의 법칙: 힘(Force)에 비례하여 힘의 방향으로 움직인다.
//   가속도 : 시간에 따라서 속도의 순간적인 변화량
// 3)작용, 반작용 법칙
//   힘(작용)을 받은 물체는 반대방향으로 힘(반작용)이 가해진다. 
void TNonCharacter::Frame(float fDeltaTime, float fGameTime)
{
    TVector2D vPos(m_rt.x1, m_rt.y1);
    //벡터의 직전의 방정식 &  시간의 동기화
    vPos = vPos + m_vDirection * m_fSpeed * fDeltaTime;

    m_fSpeed -= fDeltaTime;
    if (0 >= m_fSpeed)
    {
        m_fSpeed = rand() % 10;
    }

    if (vPos.x > 100.0f)
    {
        vPos.x = 100.0f;
        m_vDirection.x *= -1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vDirection.x *= -1.0f;
    }
    if (vPos.y > 100.0f)
    {
        vPos.y = 100.0f;
        m_vDirection.y *= -1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vDirection.y *= -1.0f;
    }
    SetPosition(vPos.x, vPos.y, m_rt.w, m_rt.h);    
}
