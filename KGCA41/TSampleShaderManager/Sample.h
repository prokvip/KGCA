#pragma once
#include "TGameCore.h"
#include "TBaseObject.h"
#include "TTextureManager.h"
#include "TShaderManager.h"

class TObject2D : public TBaseObject
{
	TShader* m_pShader = nullptr;
public:
	virtual HRESULT     CreateVertexShader(std::wstring filename);
	virtual HRESULT     CreatePixelShader(std::wstring filename);
};

class Sample : public TGameCore
{
public:
	ID3D11SamplerState* g_pDefaultSS;
	std::vector<TBaseObject*>	m_ObjectList;
	std::vector<TTexture*>		m_ObjectTextureList;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

