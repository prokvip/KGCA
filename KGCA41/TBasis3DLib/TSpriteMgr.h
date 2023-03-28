#pragma once
#include "TSprite.h"

class TSpriteMgr : public TSingleton < TSpriteMgr >
{
private:
	friend class TSingleton<TSpriteMgr>;
	ID3D11Device*			m_pd3dDevice;
public:
	typedef map <INT, TSprite*>					TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;
public:
	void			SetDevice(ID3D11Device*	 pDevice);
	INT				Add(ID3D11Device*	 pDevice,
						TCHAR *pTextureFileName,
						TCHAR *pShaderFileName,						
						ID3D11BlendState* m_pBlendState = nullptr,
						bool bInstancing = false );
	TSprite* const	GetPtr(INT iIndex);
	bool			Release();
	bool			Delete(INT iDelete);
public:
	TSpriteMgr();
	~TSpriteMgr();
};
#define I_Sprite TSpriteMgr::GetInstance()