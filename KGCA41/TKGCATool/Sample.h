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
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	virtual HRESULT		CreateDXResource() override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};

