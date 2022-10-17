#pragma once
#include "TBaseObject.h"
class TObject3D : public TBaseObject
{
public:
	TObject3D() {};
	virtual ~TObject3D() {};
};
class TShape : public TObject3D
{
public:
	TShape() {};
	virtual ~TShape() {};
};
class TShapeBox : public TObject3D
{
public:
	virtual void		CreateVertexData() override;
	virtual void		CreateIndexData() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
public:
	TShapeBox() {};
	virtual ~TShapeBox() {};
};

