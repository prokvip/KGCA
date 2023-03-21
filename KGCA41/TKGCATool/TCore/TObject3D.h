#pragma once
#include "TShape.h"
class TObject3D : public TBaseObject
{
public:
	W_STR  m_szName;
public:
	TVector3   m_vPos;
	TVector3   m_vLook;//z axis
	TVector3   m_vUp;// y
	TVector3   m_vRight; //z
	void	   SetPos(TVector3 vPos)
	{
		m_vPos = vPos;
		m_matWorld._41 = vPos.x;
		m_matWorld._42 = vPos.y;
		m_matWorld._43 = vPos.z;
		TVector3 vMin = TVector3(-10, -10, -10) + vPos;
		TVector3 vMax = TVector3(10, 10, 10) + vPos;
		m_tBox.Set(vMax, vMin);
	}
public:
	TObject3D() {}
	virtual ~TObject3D() {}
};

class TObjectBox : public TObject3D
{
public:
	TShape* m_pDirLineShape=nullptr;
public:
	bool  Init() override;
	bool  Frame() override;
	bool  Render() override;
	bool  Release() override;
	virtual void		SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
public:
	virtual void		CreateVertexData() override;
	virtual void		CreateIndexData() override;
public:
	TObjectBox() {}
	virtual ~TObjectBox() {}
};