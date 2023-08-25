#pragma once
#include "TCore.h"
#include "TSpriteObj.h"
struct TEffectInfo
{
	bool     bLifeEnable = true;
	TVector3 p;
	TVector3 s;
	TVector3 r;
	float m_fOffsetTime = 0.0f;
	float m_fAnimTimer = 0.0f;
	int   m_iCurrentAnimIndex = 0;
	W_STR    sprite;
	int    index;
	int    iMaxSize;
	void Frame()
	{
		if (iMaxSize <= 1) return;
		m_fAnimTimer += g_fSecondPerFrame;		
		if (m_fAnimTimer >= m_fOffsetTime)
		{
			m_iCurrentAnimIndex++;
			if (m_iCurrentAnimIndex >= iMaxSize)
			{
				m_iCurrentAnimIndex = 0;
				bLifeEnable = false;
			}
			m_fAnimTimer -= m_fOffsetTime;
		}
	}
	
};
class Sample : public TCore
{
	TObject* m_pMapObj = nullptr;	
	TSpriteObj* m_pSpriteTexObj = nullptr;
	TSpriteObj* m_pSpriteUVObj = nullptr;
	TSpriteObj* m_pSpriteAirObj = nullptr;	

	std::map<int, TSpriteObj*> m_SpriteList;
	std::list<TEffectInfo>     effectList;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

