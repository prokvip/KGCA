#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TCamera.h"
class Sample : public TCore
{
	TCamera		    m_Camera;
	TPlaneObj       m_MapObj;
	TPlaneObj       m_PlayerObj;
	TPlaneObj       m_ObjB;
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

