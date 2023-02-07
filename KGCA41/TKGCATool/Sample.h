#pragma once
#include "TGameCore.h"
#include "TSceneTitle.h"
#include "TSceneInGame.h"
#include "TRenderTarget.h"
#include "TQuadtree.h"
#include "TFbxFile.h"
#include "TCharacter.h"
class Sample : public TGameCore
{
public:
	std::shared_ptr<TSceneTitle>	m_pTitle = nullptr;
	std::shared_ptr<TSceneInGame>	m_pInGame = nullptr;
	std::shared_ptr<TScene>			m_pCurrentScene = nullptr;
	TShapeDirectionLine						m_DirLine;
	TQuadtree						m_Quadtree;

	//TFbxFile	m_FBXLoader;
	std::vector< TFbxFile*> m_fbxList;
	std::vector< TCharacter*> m_NpcList;
	TCharacter* m_UserCharacter;
public:
	bool		m_bWireFrame = false;
public:
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	virtual HRESULT		CreateDXResource() override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};

