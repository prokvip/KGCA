#pragma once
#include "TCore.h"
#include "TObject3D.h"
class TPlaneObj : public TObject3D
{
	TVector3			m_vCamera;
	TMatrix				m_matWorld;
	TMatrix				m_matView;
	TMatrix				m_matProj;
public:
	TPlaneObj()
	{
		m_vCamera.x = 0.0f;
		m_vCamera.y = 0.0f;
		m_vCamera.z = -5.0f;
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
		m_VertexList[0].p = TVector3(-1.0f, 1.0f, 3.5f);
		m_VertexList[0].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[0].t = TVector2(0.0f, 0.0f);

		m_VertexList[1].p = TVector3(1.0f, 1.0f, 3.5f);
		m_VertexList[1].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[1].t = TVector2(1.0f, 0.0f);


		m_VertexList[2].p = TVector3(-1.0f, -1.0f, 3.5f);
		m_VertexList[2].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[2].t = TVector2(0.0f, 1.0f);


		m_VertexList[3].p = TVector3(1.0f, -1.0f, 3.5f);
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
		
		// world
		TMatrix matRotate, matScale, matTrans;
		matRotate.ZRotate(g_fGameTimer);
		matScale.Scale(cos(g_fGameTimer)*0.5f+0.5f, 1.0f, 1.0f);
		matTrans.Translation(0,
			cos(g_fGameTimer) * 0.5f + 0.5f, 0);
		//m_matWorld = matRotate;
		// view
		if (TInput::Get().GetKey(VK_LEFT))
		{
			m_vCamera.z -= g_fSecPerFrame * 1.0f;
		}
		if (TInput::Get().GetKey(VK_RIGHT))
		{
			m_vCamera.z += g_fSecPerFrame * 1.0f;
		}

		TVector3 vTarget(0, 0, 0);
		TVector3 vUp(0, 1, 0);
		m_matView.CreateViewLook(m_vCamera, vTarget, vUp);
		// Projection
		m_matProj.PerspectiveFovLH(	1.0f, 100.0f, TBASIS_PI * 0.5f, 800.0f / 600.0f);

		m_ConstantList.matWorld = m_matWorld.Transpose();
		m_ConstantList.matView = m_matView.Transpose();;
		m_ConstantList.matProj = m_matProj.Transpose();

		TObject3D::Frame();
		return true;
	}
};

class Sample : public TCore
{
	TPlaneObj       m_Obj;
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

