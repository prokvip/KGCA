#pragma once
#include "TPlaneObj.h"
struct TSpriteInfo
{
	TVector3 p;
	TVector3 s; 
	W_STR texFile;
	W_STR texAlphaFile;
	W_STR shaderFile;
};
class TSpriteObj : public TPlaneObj
{
public:
	const TTexture* m_pAlphaTex = nullptr;
	float m_fOffsetTime = 0.0f;
	float m_fAnimTimer = 0.0f;
	int   m_iCurrentAnimIndex = 0;
public:
	virtual int    GetMaxSize() { return 1; }
	virtual bool  Load(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pImmediateContext,
		TSpriteInfo) ;

};
// 텍스쳐 교체 에니메이션
class TSpriteTexture : public TSpriteObj
{
public:
	std::vector<const TTexture*>  m_pTexList;	
public:
	virtual int    GetMaxSize() { return m_pTexList.size(); }
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;	
public:
	TSpriteTexture();
	virtual ~TSpriteTexture();
};
// UV 교체 에니메이션
class TSpriteUV : public TSpriteObj
{
public:
	std::vector<TUVRect>  m_pUVList;
public:
	virtual int    GetMaxSize() { return m_pUVList.size(); }
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	TSpriteUV();
	virtual ~TSpriteUV();
};

