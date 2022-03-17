#pragma once
#include "TCore.h"
#include "TObject3D.h"
class TPlaneObj : public TObject3D
{
public:
	virtual bool		SetVertexData()
	{
		// Local Coord
		// Box
		//  0        1 
		//     ¿øÁ¡
		//  2        3
		m_VertexList.resize(4);
		// -Z plane
		m_VertexList[0].p = TVector3(-1.0f, 1.0f, 0.5f);
		m_VertexList[0].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[0].t = TVector2(0.0f, 0.0f);

		m_VertexList[1].p = TVector3(1.0f, 1.0f, 0.5f);
		m_VertexList[1].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[1].t = TVector2(1.0f, 0.0f);


		m_VertexList[2].p = TVector3(-1.0f, -1.0f, 0.5f);
		m_VertexList[2].n = TVector3(0.0f, 0.0f, -1.0f);
		m_VertexList[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[2].t = TVector2(0.0f, 1.0f);


		m_VertexList[3].p = TVector3(1.0f, -1.0f, 0.5f);
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
		TMatrix matRotate, matScale, matTrans;
		matRotate.ZRotate(g_fGameTimer);
		matScale.Scale(cos(g_fGameTimer)*0.5f+0.5f, 1.0f, 1.0f);
		matTrans.Translation(0,
			cos(g_fGameTimer) * 0.5f + 0.5f, 0);
		m_ConstantList.matWorld = matRotate;// matScale* matRotate* matTrans;
		
		m_ConstantList.matWorld.Transpose();
		if (m_pContext != nullptr)
		{
			m_pContext->UpdateSubresource(
				m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);
		}
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

