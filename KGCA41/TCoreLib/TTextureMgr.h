#pragma once
#include "TStd.h"

class TTexture
{
	ID3D11ShaderResourceView* m_pTexSRV = nullptr;
public:
	std::wstring   m_csName;
	std::wstring   m_csPath;
	void Apply(ID3D11DeviceContext* pImmediateContext, int iSlot) const
	{
		pImmediateContext->PSSetShaderResources(iSlot, 1, &m_pTexSRV);
	}
	bool  Load(ID3D11Device* pDevice,std::wstring filename);
	bool  Release();
};
class TTextureMgr
{
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
	using tList = std::map<std::wstring, TTexture*>;
public:
	static TTextureMgr& GetInstance()
	{
		static TTextureMgr input;
		return input;
	}
	tList   m_list;
public:
    void  Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
	bool  Release();
	const TTexture* Load(std::wstring szFilepath);
	const TTexture* GetPtr(std::wstring key);
	bool	  Get(std::wstring key, TTexture& ret);
private:
	TTextureMgr();
public:
	virtual ~TTextureMgr();
};

#define I_Tex TTextureMgr::GetInstance()