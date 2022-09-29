#pragma once
#include "TSprite.h"

class TSpriteManager : public TSingleton<TSpriteManager>
{
	static TSprite  m_NullSprite;
public:
	std::vector<W_STR> m_rtNameList;
	std::vector<RECT_ARRAY> m_rtSpriteList;
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
private:
	friend class TSingleton<TSpriteManager>;
	std::map<std::wstring, std::unique_ptr<TSprite>> m_List;
	std::vector<W_STR>  filelist;
public:
	bool GameDataLoad(const TCHAR* pszLoad);
	bool Load(std::wstring name);
	TSprite* Find(std::wstring name);
	bool	  Release();
	TSprite& Get(W_STR name);
	TSprite* GetPtr(W_STR name);
private:
	TSpriteManager();
public:
	~TSpriteManager();
};

#define I_Sprite TSpriteManager::GetInstance()

