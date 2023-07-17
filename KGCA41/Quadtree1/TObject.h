#pragma once
#include "TCollision.h"
class TObject
{
public:
	std::wstring    m_csName;
	TPoint			m_Position;
	virtual void	Move(float fSecond) {};
	virtual void	SetTarget(TPoint& t) {};
};
class TStaticObject : public TObject
{
public:	
	void			Move(float fSecond) {};
};
class TDynamicObject : public TObject
{
public:
	float			m_fSpeed = 100.0f; // 1�ʴ� ���� �ִ� �Ÿ�
	TPoint			m_Direction;
	TPoint			m_Target;
public:
	void			SetTarget(TPoint& t);
	void			Move(float fSecond);
public:
	TDynamicObject();
};


