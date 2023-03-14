#pragma once
#include "TGameCore.h"
#include "TSceneTitle.h"
#include "TSceneInGame.h"
#include "TRenderTarget.h"
#include "TQuadtree.h"
#include "TFbxFile.h"
#include "TObjectManager.h"
#include "TSelect.h"
#include "TParticleObj.h"
#include "TDxRT.h"


struct SHADOW_CONSTANT_BUFFER // b1
{
	TMatrix			g_matShadow;
	TVector4        g_vLightDir[3];
	TVector4		g_vLightPos[3];
	TVector4		g_vLightColor[3];
};
class Sample : public TGameCore
{
public:
	std::shared_ptr<TSceneTitle>	m_pTitle = nullptr;
	std::shared_ptr<TSceneInGame>	m_pInGame = nullptr;
	std::shared_ptr<TScene>			m_pCurrentScene = nullptr;
	TShapeDirectionLine				m_DirLine;
	TQuadtree						m_Quadtree;
	std::vector< TFbxFile*> m_fbxList;
	std::vector< TCharacter*> m_NpcList;
	TCharacter* m_UserCharacter;
	TSelect		m_Select;
	DX::TDxRT	m_RT;
	TMatrix			m_matShadow;
	TMatrix			m_matTexture;
	TMatrix			m_matShadowView;
	TMatrix			m_matShadowProj;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> g_pSSShadowMap;
	SHADOW_CONSTANT_BUFFER			m_cbShadow;
	ComPtr<ID3D11Buffer>			m_pShadowConstantBuffer;
	D3D11_RASTERIZER_DESC			m_SlopeScaledDepthBiasDesc;
public:
	std::vector< TParticleObj*>  m_ParticleList;
	void		NewEffect(UINT iParticleCounter, T_STR tex);
public:
	bool		m_bWireFrame = false;
	bool		m_bObjectPicking = false;
	bool		m_bUpPicking = false;
	bool		m_bDownPicking = false;
	bool		m_bPlatPicking = false;
	T_STR		m_szSelectFbxFile;
public:
	bool		GetIntersection();
	bool		CreateMapData(UINT iColumn = 257, UINT iRows = 257);
	bool		CreateFbxLoader();
	bool		LoadFbx(T_STR filepath, TVector3 vPos);
	bool		ObjectRender();
	void		PreDepthShadow();
	void		InitRT();
	bool		ObjectShadow();
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	virtual HRESULT		CreateDXResource() override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};

