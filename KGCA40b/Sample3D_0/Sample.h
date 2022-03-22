#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TCamera.h"
#include "TMap.h"
#include "TSkyObj.h"
#include "TBoxObj.h"
class Sample : public TCore
{
	TCamera		    m_Camera;
	TMap			m_MapObj;
	TBoxObj			m_PlayerObj;
	TPlaneObj       m_ObjB;
	TSkyObj			m_SkyObj;
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

