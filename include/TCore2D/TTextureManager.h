#pragma once
#include "TTexture.h"
class TTextureManager : public TSingleton<TTextureManager>
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
private:
	friend class TSingleton<TTextureManager>;
	std::map<std::wstring, TTexture*> m_List;
public:
	TTexture* Load(std::wstring name);
	TTexture* Find(std::wstring name);
	bool	  Release();
private:
	TTextureManager();
public:
	~TTextureManager();
};

#define I_Tex TTextureManager::GetInstance()
