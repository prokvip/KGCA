#pragma once
#include "TMapObject.h"
#include "TUser2D.h"
#include "TNpc2D.h"
class TScene 
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
	TUser2D* m_pUser;
	TMapObject* m_pMap;
	std::vector<TBaseObject*>	m_pNpcList;
	std::vector<TTexture*>		m_ObjectTextureList;
public:
	virtual bool		Create(
		ID3D11Device* pd3dDevice,// 디바이스 객체
		ID3D11DeviceContext* pImmediateContext,
		std::wstring shadername);
public:
	virtual bool		Init() ;
	virtual bool		Frame() ;
	virtual bool		Render() ;
	virtual bool		Release() ;
};

