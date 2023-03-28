#pragma once
#include "../TBaseObj.h"
#include "../TBoxObj.h"

#define NUM_OBJECTS 1000

class TMapObject : public TBaseObj
{
public:
	TMatrix				m_matWorld;	
	TVector3			m_vPosition;
	TVector4			m_vColor;
	TBoxObj*					m_pBoxObj;
public:
	void				Update();	
	void				SetPosition( TVector3 vPos );
public:
	TMapObject();
	~TMapObject();
};