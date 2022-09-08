#pragma once
#include "TGameCore.h"
#include "TBaseObject.h"
#include "TTextureManager.h"

class Sample : public TGameCore
{
public:
	ID3D11SamplerState*			g_pDefaultSS;
	TTexture					m_Texture;
	std::vector<TBaseObject*>	m_ObjectList;
	std::vector<TTexture*>		m_ObjectTextureList;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

