#pragma once
#include "TCore.h"
#include "TFbxObj.h"
#include "TMap.h"
#include "TQuadtree.h"
class TSampleMap : public TMap
{
public:
	virtual float		GetHeight(int index) override
	{
		return TMap::GetHeight(index) * 0.1f;
	}
	virtual ~TSampleMap() {}
};
class Sample : public TCore
{
	TSampleMap				m_MapObj;
	TQuadtree				m_Quadtree;
	std::vector<TFbx>		m_FbxObj;
	TTexture* m_pLightTex;
public:
	virtual void	CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void	DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;	
public:
	bool	LoadMap();
	bool    LoadFbx();
public:
	Sample();
	virtual ~Sample();
};
