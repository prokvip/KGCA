#pragma once
#include "TShape.h"
class TObject3D : public TBaseObject
{
public:
	W_STR     m_csName;
public:
	TVector   m_vPos;
	TVector   m_vLook;//z axis
	TVector   m_vUp;// y
	TVector   m_vRight; //z
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