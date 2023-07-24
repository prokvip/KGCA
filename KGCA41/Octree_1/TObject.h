#pragma once
#include "TCollision.h"
class TObject2D
{
public:
	std::wstring    m_csName;
	TPoint2			m_Position;
	TRect			m_tRT;
	virtual void    SetRect(TPoint2& c, float w, float h);
	virtual void	Move(float fSecond) {};
	virtual void	SetTarget(TPoint2& t) {};
};
class TStaticObject2D : public TObject2D
{
public:	
	void			Move(float fSecond) {};
};
class TDynamicObject2D : public TObject2D
{
public:
	float			m_fSpeed = 100.0f; // 1초당 갈수 있는 거리
	TPoint2			m_Direction;
	TPoint2			m_Target;
public:
	void			SetTarget(TPoint2& t);
	void			Move(float fSecond);
public:
	TDynamicObject2D();
};

class TObject3D
{
public:
	int				m_iNodeIndex = -1;
	std::wstring    m_csName;
	TPoint3			m_Position;
	TBox			m_tBox;
	virtual void    SetBox(TPoint3& c, float w, float h, float z);
	virtual void	Move(float fSecond) {};
	virtual void	SetTarget(TPoint3& t) {};
};
class TStaticObject3D : public TObject3D
{
public:
	void			Move(float fSecond) {};
};
class TDynamicObject3D : public TObject3D
{
public:
	float			m_fSpeed = 100.0f; // 1초당 갈수 있는 거리
	TPoint3			m_Direction;
	TPoint3			m_Target;
public:
	void			SetTarget(TPoint3& t);
	void			Move(float fSecond);
public:
	TDynamicObject3D();
};


