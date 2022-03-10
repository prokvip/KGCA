#include "TPlayerObj2D.h"
#include "TInput.h"

bool	TPlayerObj2D::Frame()
{	
	if (TInput::Get().GetKey('W') == KEY_HOLD)
	{
		TVector2 pos;
		// �ð��� ����ȭ
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
	m_fSpeed = 600.0f; // �ӷ�
}

TPlayerObj2D::~TPlayerObj2D()
{	
}