#pragma once
#include "TGameCore.h"
#include "TMapObject.h"
#include "TUser2D.h"
#include "TNpc2D.h"
class Sample : public TGameCore
{
public:
	TUser2D*			m_pUser;
	TMapObject*			m_pMap;
	std::vector<TBaseObject*>	m_pNpcList;
	std::vector<TTexture*>		m_ObjectTextureList;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

