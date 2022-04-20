#pragma once
#include "TCore.h"
#include "TFbxObj.h"
#include "TMap.h"
#include "TQuadtree.h"
#include "TQuadObject.h"
#include "TDxRT.h"
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
	TTexture*	m_pLightTex;
	TTexture*	m_pNormalMap;
	bool		m_bDepthShadow = false;
public:
	TShader*	m_pProjShadowVShader = nullptr;
	TShader*	m_pProjShadowPShader = nullptr;
	TDxRT		m_dxRT;
	TVector3	m_vLightPos;
	TVector3	m_vLightDir;
	TMatrix		m_matShadow;
	TMatrix		m_matViewLight;
	TMatrix		m_matProjLight;
	TMatrix		m_matTex;
	void		RenderDepthShadow(TMatrix* matView, TMatrix* matProj);
	void		RenderProjectionShadow(TMatrix* matView, TMatrix* matProj);
public:
	TShader*	m_pDepthShadowVShader = nullptr;
	TShader*	m_pDepthShadowPShader = nullptr;
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

