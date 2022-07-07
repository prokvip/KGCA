#pragma once
#include "TObject2D.h"
class TSprite : public TObject2D
{
public:
	float			   m_fAnimTime;
	std::vector<RECT>  m_rtArray;
public:
	void			SetRectSouceArray(RECT rt);
	virtual bool	Init() override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool	Release()override;
public:
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
};

class TSpriteMgr : public TBaseMgr<TSprite, TSpriteMgr>
{
	friend class TSingleton<TSpriteMgr>;
public:
	TSprite*	Load(std::wstring filename) override;
private:
	TSpriteMgr() {};
public:
	~TSpriteMgr() {};
};
#define I_Sprite TSpriteMgr::Get()
