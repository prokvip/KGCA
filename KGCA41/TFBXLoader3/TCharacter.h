#pragma once
#include "TFbxFile.h"
struct TActionTable
{
	UINT iStartFrame;
	UINT iEndFrame;
	float fTickPerFrame; // 160
	float fFrameSpeed; // 30
	bool  bLoop;
	TActionTable()
	{
		bLoop = false;
	}
};
class TCharacter
{
public:
	UINT	    m_iFbxListID;
	TFbxFile* m_pFbxFile = nullptr;
public:
	TMatrix     m_matWorld;
	TMatrix     m_matView;
	TMatrix     m_matProj;
public:
	TActionTable m_ActionCurrent;
	TAnimScene  m_AnimScene;
	float       m_fAnimFrame = 0;
	UINT		m_iStartFrame;
	UINT		m_iEndFrame;
	float       m_fAnimInverse = 1.0f;
	float       m_fAnimSpeed = 1.0f;

	std::map<std::wstring, TActionTable> m_ActionList;

	VS_CONSTANT_BONE_BUFFER  m_cbDataBone;
	std::vector< VS_CONSTANT_BONE_BUFFER> m_cbDrawGeom;

	ID3D11Buffer* m_pAnimBoneCB;
	std::vector< ID3D11Buffer*>m_pSkinBoneCB;
public:
	HRESULT	CreateConstantBuffer(ID3D11Device* pDevice);
	bool UpdateFrame(ID3D11DeviceContext* pContext);
	void	SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
};
