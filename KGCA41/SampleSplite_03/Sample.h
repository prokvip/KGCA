#pragma once
#include "TCore.h"
#include "TSpriteObj.h"
class TParticle
{
public:
	bool     m_bLife = true;
	TVector3 m_vPos;
	TVector3 m_vSclae;
	TVector3 m_vRotation;
	float	m_fOffsetTime = 0.0f;
	float	m_fAnimTimer = 0.0f;
	int		m_iCurrentAnimIndex = 0;
	W_STR   m_szName;
	int		m_iID;
	int		m_iMaxFrame;
	void Frame()
	{
		if (m_iMaxFrame <= 1 || !m_bLife) return;
		m_fAnimTimer += g_fSecondPerFrame;		
		if (m_fAnimTimer >= m_fOffsetTime)
		{
			m_iCurrentAnimIndex++;
			if (m_iCurrentAnimIndex >= m_iMaxFrame)
			{
				m_iCurrentAnimIndex = 0;
				m_bLife = false;
			}
			m_fAnimTimer -= m_fOffsetTime;
		}
	}	
};

class Sample : public TCore
{
	using tPlaneObj = std::unique_ptr<TPlaneObj>;
	using tSpriteObj = std::unique_ptr<TSpriteObj>;
	tPlaneObj  m_pMapObj = nullptr;
	tSpriteObj m_pSpriteTexObj = nullptr;
	tSpriteObj m_pSpriteUVObj = nullptr;
	tSpriteObj m_pSpriteAirObj = nullptr;

	std::map<int, TSpriteObj*>	m_SpriteList;
	std::list<TParticle>		m_ParticleList;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

