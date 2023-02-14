#pragma once
#include "TScene.h"
#include "TInterface.h"
#include "TProjectile2D.h"
#include "TObject3D.h"
#include "TCameraDebug.h"
#include "TCameraMap.h"
#include "TMap.h"
#include "TInput.h"
class TUser : public TObjectBox
{
public:
	bool Frame()
	{		
		if (I_Input.GetKey('W') == KEY_HOLD)
		{
			TVector3 v = m_vLook * g_fSecondPerFrame * 50.0f;
			m_vPos += v;
		}
		if (I_Input.GetKey('S') == KEY_HOLD)
		{
			TVector3 v = -m_vLook * g_fSecondPerFrame * 50.0f;
			m_vPos += v;
		}
		
	/*	if (I_Input.GetKey('Q') == KEY_HOLD)
		{
			TVector3 v = m_vUp * m_fSpeed * g_fSecondPerFrame* 50.0f;
			m_vPos += v;
		}
		if (I_Input.GetKey('E') == KEY_HOLD)
		{
			TVector3 v = m_vUp * -m_fSpeed * g_fSecondPerFrame* 50.0f;
			m_vPos += v;
		}*/

		D3DXMatrixTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
		Update();
		return true;
	}

	void Update()
	{
		m_vRight.x = m_matWorld._11;
		m_vRight.y = m_matWorld._12;
		m_vRight.z = m_matWorld._13;

		m_vUp.x = m_matWorld._21;
		m_vUp.y = m_matWorld._22;
		m_vUp.z = m_matWorld._23;

		m_vLook.x = m_matWorld._31;
		m_vLook.y = m_matWorld._32;
		m_vLook.z = m_matWorld._33;

		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Normalize(&m_vLook, &m_vLook);		
	}
};
class TSceneTitle :   public TScene
{
public:
	TVector3        m_vBeforePos;
	TBaseObject*	m_pBG;
	TMap* m_pMap = nullptr;
	TUser*			m_pUser=nullptr;
	TCamera* m_pMapCamera=nullptr;
	TCamera* m_pDebugCamera = nullptr;
public:
	bool				CreateMap(UINT iColumn=257, UINT iRows=257);
	bool				DeletaMap();
	bool				CreatePlayer();
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual bool        IsNextScene();
	
};

