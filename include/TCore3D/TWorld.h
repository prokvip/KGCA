#pragma once
#include "TObject2D.h"
class TWorld
{
public:
	enum TWorldID{ T_LOADING, T_LOGIN, T_LOBBY, T_ZONE, T_RESULT, };
	ID3D11Device*		 m_pd3dDevice;	// 디바이스 객체
	ID3D11DeviceContext* m_pContext;// 다비이스 컨텍스트 객체
	bool		m_bLoadZone = false;
public:
	static TWorld* m_pWorld;
	std::vector<std::shared_ptr<TObject2D>> m_UIObj;
	std::vector<std::shared_ptr<TObject2D>> m_ItemObj;
	std::vector<std::shared_ptr<TObject2D>> m_NpcObj;
	std::vector<std::shared_ptr<TObject2D>> m_MapObj;
	using m_mapIter = std::vector<std::shared_ptr<TObject2D>>::iterator;
public:
	virtual bool	Load(std::wstring saveFile);
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	TWorld();
	virtual ~TWorld();
};

