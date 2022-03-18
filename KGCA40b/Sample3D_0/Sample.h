#pragma once
#include "TCore.h"
#include "TObject3D.h"
#include "TCamera.h"
class TPlaneObj : public TObject3D
{
public:
	TMatrix				m_matWorld;
public:
	TPlaneObj()
	{		
	}
	virtual void		SetMatrix(TMatrix* matWorld,
		TMatrix* matView, TMatrix* matProj)
	{
		m_ConstantList.matWorld = m_matWorld.Transpose();
		if (matWorld != nullptr)
		{
			m_ConstantList.matWorld = matWorld->Transpose();
		}
		if (matView != nullptr)
		{
			m_ConstantList.matView = matView->Transpose();
		}
		if (matProj!=nullptr)
		{
			m_ConstantList.matProj = matProj->Transpose();
		}
	}

	virtual bool		SetVertexData()
	{
		// Local Coord
		// Box
		//  0        1 
		//     ¿øÁ¡
		//  2        3
		m_VertexList.resize(4);
		// -Z plane
		m_VertexList[0].p = TVector3(-1.0f, 1.0f, 0.0f);
		m_VertexList[0].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[0].t = TVector2(0.0f, 0.0f);

		m_VertexList[1].p = TVector3(1.0f, 1.0f, 0.0f);
		m_VertexList[1].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[1].t = TVector2(1.0f, 0.0f);


		m_VertexList[2].p = TVector3(-1.0f, -1.0f, 0.0f);
		m_VertexList[2].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[2].t = TVector2(0.0f, 1.0f);


		m_VertexList[3].p = TVector3(1.0f, -1.0f, 0.0f);
		m_VertexList[3].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[3].t = TVector2(1.0f, 1.0f);
		return true;
	}
	virtual bool		SetIndexData()
	{
		m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
		m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
		return true;
	}
	virtual bool        Frame()
	{			
		TObject3D::Frame();
		return true;
	}
};

class Sample : public TCore
{
	TCamera		    m_Camera;
	TPlaneObj       m_ObjA;
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

