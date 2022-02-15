#include "TPlayerObj2D.h"
#include "TInput.h"
void TPlayerObj2D::HitOverlap(TBaseObject* pObj, DWORD dwState)
{
	if (dwState == TCollisionType::Overlap)
	{
		pObj->m_bAlphaBlend = false;
	}	
}
bool	TPlayerObj2D::Frame()
{	
	if (TInput::Get().GetKey('W') == KEY_HOLD)
	{
		TVector2 pos;
		// 시간의 동기화
		pos.y -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (TInput::Get().GetKey('A') == KEY_HOLD)
	{
		TVector2 pos;
		pos.x -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (TInput::Get().GetKey('S') == KEY_HOLD)
	{
		TVector2 pos;
		pos.y += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (TInput::Get().GetKey('D') == KEY_HOLD)
	{
		TVector2 pos;
		pos.x += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}	
	return true;
}

TPlayerObj2D::TPlayerObj2D()
{
	m_fSpeed = 300.0f; // 속력
}

TPlayerObj2D::~TPlayerObj2D()
{	
}