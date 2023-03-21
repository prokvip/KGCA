#pragma once
#include "TBaseObject.h"

class TShape : public TBaseObject
{
public:
	TShape() {};
	virtual ~TShape() {};
};
class TShapeBox : public TShape
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

class TShapeLine : public TShape
{
public:
	virtual void		CreateVertexData() override;
	virtual void		CreateIndexData() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
public:
	TShapeLine() {};
	virtual ~TShapeLine() {};
};


class TShapeDirectionLine : public TShapeLine
{
public:
	virtual void		CreateVertexData() override;
	virtual void		CreateIndexData() override;
public:
	TShapeDirectionLine() {};
	virtual ~TShapeDirectionLine() {};
};