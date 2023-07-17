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
	float			m_fSpeed = 100.0f; // 1초당 갈수 있는 거리
	TPoint			m_Direction;
	TPoint			m_Target;
public:
	void			SetTarget(TPoint& t);
	void			Move(float fSecond);
public:
	TDynamicObject();
};


