#pragma once
#include "TCore.h"
#include "TFbxObj.h"
#include "TMap.h"
#include "TQuadtree.h"
#include "TQuadObject.h"
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
	TQuadObject				m_QuadObj;
	TShader*				m_pShadowPShader = nullptr;
	TTexture* m_pLightTex;
	TTexture* m_pNormalMap;
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
	void	DisplayErrorBox(const WCHAR* lpszFunction);
	DWORD	LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list);
public:
	void		RenderIntoBuffer(ID3D11DeviceContext* pContext);
	void		RenderMRT(ID3D11DeviceContext* pContext);
public:
	Sample();
	virtual ~Sample();
};

