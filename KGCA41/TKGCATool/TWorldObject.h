#pragma once
#include "TCharacter.h"
class TWorldObject : public TObject3D
{
public:
	TCharacter* m_pCharacter = nullptr;
	TFbxFile*	m_pFbxFile = nullptr;
public:
	TActionTable m_ActionCurrent;
	TAnimScene  m_AnimScene;
	float       m_fAnimFrame = 0;
	UINT		m_iStartFrame;
	UINT		m_iEndFrame;
	float       m_fAnimInverse = 1.0f;
	float       m_fAnimSpeed = 1.0f;

	TFbxFile* m_pAnionFbxFile = nullptr;
	std::map<std::wstring, TFbxFile*>    m_ActionFileList;
	std::map<std::wstring, TActionTable> m_ActionList;

	VS_CONSTANT_BONE_BUFFER  m_cbDataBone;
	std::vector< VS_CONSTANT_BONE_BUFFER> m_cbDrawGeom;

	ID3D11Buffer* m_pAnimBoneCB;
	std::vector< ID3D11Buffer*>m_pSkinBoneCB;
public:
	bool	UpdateFrame(ID3D11DeviceContext* pContext);
public:
	HRESULT	CreateConstantBuffer()override;
	void	SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj) override;
	bool	PreRender(ID3D11DeviceContext* pContext) override;
	bool	Render(ID3D11DeviceContext* pContext) override;
	bool	RenderShadow(ID3D11DeviceContext* pContext)override;
	bool	PostRender(ID3D11DeviceContext* pContext) override;
	bool	Release() override;
	bool	Load(ID3D11Device*, ID3D11DeviceContext*, TCharacter* , T_STR);
public:
	TWorldObject();
	virtual ~TWorldObject(); 
};

