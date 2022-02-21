#pragma once
#include "TObject2D.h"
class TWorld
{
public:
	enum TWorldID{ T_LOADING, T_LOGIN, T_LOBBY, T_ZONE, T_RESULT, };
	ID3D11Device*		 m_pd3dDevice;	// 디바이스 객체
	ID3D11DeviceContext* m_pContext;// 다비이스 컨텍스트 객체
public:
	static TWorld* m_pWorld;
	std::map<std::wstring, TObject2D*> m_UIObj;
	std::map<std::wstring, TObject2D*> m_ItemObj;
	std::map<std::wstring, TObject2D*> m_NpcObj;	
	std::map<std::wstring, TObject2D*> m_MapObj;
	using m_mapIter = std::map<std::wstring, TObject2D*>::iterator;
public:
	virtual bool	Load(std::wstring saveFile);
	virtual bool	Init() = 0;
	virtual bool	Frame() = 0;
	virtual bool	Render() = 0;
	virtual bool	Release();
public:
	TWorld();
	virtual ~TWorld();
};

