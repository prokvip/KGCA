#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TCamera.h"
#include "TMap.h"
#include "TSkyObj.h"
#include "TBoxObj.h"
#include "TQuadtree.h" 

#define  MAX_NUM_OBJECTS 100
class Sample : public TCore
{
	TCamera		    m_Camera;
	TCamera			m_CameraTopView;
	TMap			m_MapObj;
	TBoxObj			m_PlayerObj;
	std::vector<TMapObject*> m_pObjList;
	TSkyObj			m_SkyObj;
	TQuadtree		m_Quadtree;
	TBoxObj*		m_pBoxObj;
public:
	void    CreateMapObject();
	virtual void	CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void	DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	bool    MiniMapRender();
	virtual bool	Release()  override;	
public:
	Sample();
	virtual ~Sample();
};

