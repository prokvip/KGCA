#pragma once
#include "TObject2D.h"
#include "TSoundMgr.h"
struct TStatePlayData
{
	TTexture* pTex;
	TSound* pSound;
	TStatePlayData(const TStatePlayData& data)
	{
		pTex = data.pTex;
		pSound = data.pSound;
		DisplayText("TStatePlayDataCopy\n");
	}
	TStatePlayData()
	{
		DisplayText("TStatePlayData\n");
	}
	TStatePlayData(TTexture* a,TSound* b) 
	{
		pTex = a;
		pSound = b;
		DisplayText("TStatePlayData\n");
	}
};
class TUIObject : public TObject2D
{
public:
	std::vector<TStatePlayData> m_pStatePlayList;
	RECT	m_rtOffset;
public:	
	bool	Frame() override;
	bool	Render()override;
	bool    SetVertexData()override;
	bool    SetIndexData()override;
};
class TImageObject : public TUIObject
{
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
};
class TButtonObject : public TUIObject
{
public:
	virtual void	HitSelect(TBaseObject* pObj, DWORD dwState) override;
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
};
