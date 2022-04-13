#pragma once
#include "TCore.h"
#include "TFbxObj.h"
class Sample : public TCore
{
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
	Sample();
	virtual ~Sample();
};

