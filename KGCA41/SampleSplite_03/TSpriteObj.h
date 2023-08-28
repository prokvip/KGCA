#pragma once
#include "TPlaneObj.h"
struct TSpriteInfo
{
	int			iNumRow;
	int			iNumColumn;
	TVector3	p;
	TVector3	s; 
	float		fAnimTimer;
	float		fElapsedTimer; // 에니메이션 가동 시간
	W_STR		texFile;
	W_STR		texAlphaFile;
	W_STR		shaderFile;
	T_STR_VECTOR texList;
	void Reset()
	{
		iNumRow = 1;
		iNumColumn = 1;
		fAnimTimer = 1.0f;
		fElapsedTimer = 0.0f;
		p = { 1.0f, 1.0f, 1.0f };
		s = { 0.0f, 0.0f, 0.0f };
		texList.clear();
		texFile = L"";
		texAlphaFile = L"";
		shaderFile = L"";
	}
	TSpriteInfo()
	{
		Reset();
	}
};
class TSpriteObj : public TPlaneObj
{
public:
	const	TTexture* m_pAlphaTex = nullptr;
	float	m_fAnimTimer = 1.0f;  // 전체 시간
	float	m_fOffsetTime = 0.0f;	// 1프레임 교체시간
	float	m_fElapsedTimer = 0.0f; // 누적시간
	int		m_iCurrentAnimIndex = 0;
	int		m_iNumSpriteX = 1;
	int		m_iNumSpriteY = 1;
	TSpriteInfo m_InitSpriteInfo;
public:
	virtual bool   Render() override;
	virtual int    GetMaxSize() { return 1; }
	virtual bool  Load(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pImmediateContext,
		TSpriteInfo) ;
	virtual bool  LoadTexArray(T_STR_VECTOR& texList) { return true; };
	virtual void  SetUVFrame(int iNumRow, int iNumColumn) {}

};
// 텍스쳐 교체 에니메이션
class TSpriteTexture : public TSpriteObj
{
public:
	std::vector<const TTexture*>  m_pTexList;	
public:
	virtual int    GetMaxSize() { return m_pTexList.size(); }
	virtual bool   LoadTexArray(T_STR_VECTOR& texList) override;
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
	void  SetUVFrame(int iNumRow, int iNumColumn) override;
	void  SetNumSprite(int x, int y)
	{
		m_iNumSpriteX = 1;
		m_iNumSpriteY = 1;
	}
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

