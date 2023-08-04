#pragma once
#include "TCollision.h"
const float g_fMapSize = 1000.0f;

class TObject2D
{
public:
	std::wstring    m_csName;
	TVector2			m_Position;
	TRect			m_tRT;
	virtual void    SetRect(TVector2& c, float w, float h);
	virtual void	Move(float fSecond) {};
	virtual void	SetTarget(TVector2& t) {};
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
	TVector2			m_Direction;
	TVector2			m_Target;
public:
	void			SetTarget(TVector2& t);
	void			Move(float fSecond);
public:
	TDynamicObject2D();
};

class TObject3D
{
public:
	int				m_iNodeIndex = -1;
	std::wstring    m_csName;
	TVector3			m_Position;
	TBox			m_tBox;
	virtual void    SetBox(TVector3& c, float w, float h, float z);
	virtual void	Move(float fSecond) {};
	virtual void	SetTarget(TVector3& t) {};
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
	TVector3			m_Direction;
	TVector3			m_Target;
public:
	void			SetTarget(TVector3& t);
	void			Move(float fSecond);
public:
	TDynamicObject3D();
};


